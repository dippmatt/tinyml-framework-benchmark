from shared_scripts.flash_ram_from_elf import calc_flash_ram
from pathlib import Path
import numpy as np
import shutil
import subprocess
from os import chdir
import re

# Idea: use cube_prj to run 2 inference loops: one with layer-by-layer measurements,
# and one with whole model measurements. Then, use the layer-by-layer measurements
# to estimate the overhead of the measurement process.

def copy_build_compile(workdir: Path, repetitions: int, input_tensors, input_dtype, output_shape: tuple, cube_template: Path, cube_template_no_ir: Path, cube_template_ref: Path, cube_template_empty: Path, bundle_dir: Path, bundle_dir_no_ir: Path):
    """Copy all data into the cube project, build and compile it
    Repeat for the inference tempalate, the reference template and the empty template.
    Inference Template: The template to run measurements.
    Reference Template: The template to measure the overhead of layer-by-layer measurements.
    Empty Template: The template to measure the RAM and FLASH usage of the model.
    Args:
        workdir: The working directory.
        cube_template: The path to the STM Cube IDE Inference Template project.
        cube_template_no_ir: The path to the STM Cube IDE Inference Template project, 
            used without per layer measurements.
        cube_template_ref: The path to the STM Cube IDE Reference Template project.
        cube_template_empty: The path to the STM Cube IDE Empty Template project.
        repetitions: The number of repetitions to run the inference per input tensor.
        input_tensors: The input tensors.
        input_dtype: The data type of the input tensors.
        bundle_dir: The path to the framework bundle.
    """
    step_output = dict()
    
    # generate input test tensors as C header from npz data
    if input_dtype in [np.float16, np.float32, np.float64, np.float128]:
        io_dtype = 'float'
    elif input_dtype in [np.uint8, np.uint16, np.uint32, np.uint64]:
        io_dtype = 'uint8_t'
    else:
        io_dtype = 'int8_t'
    test_tensor_header = gen_test_tensors(input_tensors, io_dtype)
    # copy test tensors into project only, not into reference project,
    # because the tensors are not part of the model
    data_header = cube_template / Path("Core", "Inc", "ml_data.h")
    with open(data_header, 'w') as f:
        f.writelines(test_tensor_header)
    assert data_header.exists(), f"{data_header} does not exist or is not a file"

    data_header_no_ir = cube_template_no_ir / Path("Core", "Inc", "ml_data.h")
    with open(data_header_no_ir, 'w') as f:
        f.writelines(test_tensor_header)

    # copy framework bundle into project and reference project, 
    # not into empty project because we want to measure the RAM and FLASH usage of the model
    src_files = ["model.o"]
    inc_files = ["model.h", "model.weights.txt"]
    
    # copy files with layer by layer measurements into inference template
    for inc_file in inc_files:
        file = bundle_dir / Path(inc_file)
        if inc_file == "model.h":
            input_name, output_name = get_io_names(file)
        shutil.copy(file, cube_template / Path("Core", "Inc"))
    for src_file in src_files:
        file = bundle_dir / Path(src_file)
        shutil.copy(file, cube_template / Path("Core", "Src"))

    # copy files without layer by layer measurements into empty template
    for inc_file in inc_files:
        file = bundle_dir_no_ir / Path(inc_file)
        if inc_file == "model.h":
            input_name, output_name = get_io_names(file)
        shutil.copy(file, cube_template_no_ir / Path("Core", "Inc"))
        shutil.copy(file, cube_template_ref / Path("Core", "Inc"))
    for src_file in src_files:
        file = bundle_dir_no_ir / Path(src_file)
        shutil.copy(file, cube_template_no_ir / Path("Core", "Src"))
        shutil.copy(file, cube_template_ref / Path("Core", "Src"))

    main_c = cube_template / Path("Core", "Src", "main.c")
    main_c_no_ir = cube_template_no_ir / Path("Core", "Src", "main.c")
    main_c_ref = cube_template_ref / Path("Core", "Src", "main.c")

    # insert correct model input and output layer names into main.c template
    set_io_names(main_c, input_name, output_name)
    set_io_names(main_c_no_ir, input_name, output_name)
    set_io_names(main_c_ref, input_name, output_name)
    
    # set the number of repetitions for each input tensor propagated through the model.
    # can be used for averaging the inference time over multiple runs.
    set_reps(main_c, repetitions)
    set_reps(main_c_no_ir, repetitions)
    set_reps(main_c_ref, repetitions)
    # explicitly define memory buffer allocation input / output tensors
    set_io_byte_consts(main_c, io_dtype, output_shape)
    set_io_byte_consts(main_c_no_ir, io_dtype, output_shape)
    set_io_byte_consts(main_c_ref, io_dtype, output_shape)
    # delete callbacks from main.c for no_ir template
    delete_callbacks(main_c_no_ir)

    # compile projects
    templates = [cube_template, cube_template_no_ir, cube_template_ref, cube_template_empty]
    template_names = ["cube_template", "cube_template_no_ir", "cube_template_ref", "cube_template_empty"]
    for i, template in enumerate(templates):
        build_dir = template / Path("Debug")
        chdir(build_dir)
        logfile = build_dir / Path("compile.log")
        assert subprocess.call("make all > compile.log", shell=True) == 0, f"Compiling model project encountered an error. See {logfile} for details."
        prj_name = template.name.rstrip('_no_ir')
        elf_file = build_dir / Path(f"{prj_name}.elf")
        assert elf_file.exists(), f"Compiling model project encountered an error. See {logfile} for details."
        step_output[template_names[i]] = elf_file

    ram, flash = calc_flash_ram_diff(step_output["cube_template_ref"], step_output["cube_template_empty"])
    step_output["flash"] = flash
    step_output["ram"] = ram

    return step_output

    


def gen_test_tensors(input_tensors, input_dtype):
    """Create test tensors as C arrays

    Creates a C header file with the test tensors as C arrays.
    The header file is saved in the STM Cube project's Inc folder.
    Args:
        input_tensors: A np array with the test tensors.
        input_dtype: The data type of the input tensors.
    
    Returns: A list of lines, content of the header file.
    """
    flat_tensor_len = 1
    num_samples = input_tensors.shape[0]
    for shape in input_tensors.shape[1:]:
        flat_tensor_len *= shape
    
    lines_o = []
    lines_o.append("#ifndef ML_DATA_H\n")
    lines_o.append("#define ML_DATA_H\n")
    lines_o.append("\n")
    lines_o.append("#include <stdint.h>\n")
    lines_o.append(f"#define ROWS {num_samples}\n")
    lines_o.append(f"#define COLS {flat_tensor_len}\n")
    lines_o.append("\n")
    lines_o.append("#endif /* ML_DATA_H */\n")
    lines_o.append("\n")

    lines_o.append(f"const {input_dtype} array[ROWS][COLS] = {{\n")
    for i, tensor in enumerate(input_tensors):
        values = ""
        elem = tensor.flatten()
        for number in elem:
            if input_dtype == 'int8_t':
                values += f'{number}, '
            else:
                values += f'{number: .16e}f, '
        values = values.rstrip(', ')
        lines_o.append(f"    {{ {values} }},\n")
    lines_o[-1] = lines_o[-1].rstrip(',\n') + '\n'
    lines_o.append('};\n')
    return lines_o

def get_io_names(model_h: str):
    assert model_h.exists(), f"{model_h} does not exist or is not a file"
    
    with open(model_h, 'r') as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            if "// Placeholder address offsets within mutable buffer (bytes)." in line:
                input_layer = lines[i+1] 
                output_layer = lines[i+2]
                break

        # Example for input / output layer constant offset defines,
        # we want the name of the preprocessor constant
        #define MODEL_serving_default_input_1_0  0
        #define MODEL_StatefulPartitionedCall_0  1984
        input_layer = input_layer.split()
        input_name = input_layer[1] 
        output_layer = output_layer.split()
        output_name = output_layer[1] 
    return input_name, output_name

def set_io_names(main_c: Path, input_name: str, output_name: str):
    """Set the input and output layer names in the main.c template file."""
    assert main_c.exists(), f"{main_c} does not exist or is not a file"
    
    with open(main_c, 'r') as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            if "<input_name>" in line:
                lines[i] = line.replace("<input_name>", input_name)
            if "<output_name>" in line:
                lines[i] = line.replace("<output_name>", output_name)

    with open(main_c, 'w') as file:
        file.writelines(lines)
    return

def set_reps(main_c: Path, repetitions: int):
    assert main_c.exists(), f"{main_c} does not exist or is not a file"

    with open(main_c, 'r') as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            if "<REPETITIONS_PER_INPUT_TENSOR>" in line:
                lines[i] = line.replace("<REPETITIONS_PER_INPUT_TENSOR>", str(repetitions))
                
    with open(main_c, 'w') as file:
        file.writelines(lines)
    return

def set_io_byte_consts(main_c: Path, c_out_dtype: str, output_shape: tuple):
    assert main_c.exists(), f"{main_c} does not exist or is not a file"
        
    # multiply all dimensions to get the total number of elements in the output tensor
    output_elements = 1
    for dimension in output_shape:
        output_elements *= dimension    

    if c_out_dtype == 'float':
        c_out_specifier = 'f'
    elif c_out_dtype == 'int8_t':
        c_out_specifier = 'd'
    else:
        c_out_specifier = 'u'

    with open(main_c, 'r') as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            # set flattened output tensor size
            if "<ELEMENTS>" in line:
                line = line.replace("<ELEMENTS>", str(output_elements))
            if "<o_type>" in line:
                line = line.replace("<o_type>", c_out_dtype)
            if "<int_type>" in line:
                # special case when using float model:
                # we don't need this funciton so just use uint8_t by default
                # if we were to use float we would have a function redefinition error
                if c_out_dtype == 'uint8_t':
                    line = line.replace("<int_type>", c_out_dtype)
                else:
                    line = line.replace("<int_type>", 'int8_t')
            if "<o_format_specifier>" in line:
                line = line.replace("<o_format_specifier>", c_out_specifier)
            lines[i] = line
                
    with open(main_c, 'w') as file:
        file.writelines(lines)
    return

def delete_callbacks(main_c: Path):
    new_lines = []
    lines_to_skip = 0
    with open(main_c, 'r') as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            # instrument callbacks are each 3 lines long
            if "glow_instrument_before" in line or "glow_instrument_after" in line:
                lines_to_skip = 2
            elif lines_to_skip > 0:
                lines_to_skip -= 1
            else:
                new_lines.append(line)

    with open(main_c, 'w') as file:
        file.writelines(new_lines)
    return

def calc_flash_ram_diff(elf_file_ref: Path, elf_file_empty: Path):
    """Calculate the flash and RAM usage of the project.

    Returns: A tuple with the flash and RAM usage.
    """
    # extract ram and flash usage from elf files
    ram_usage_ref, flash_usage_ref = calc_flash_ram(elf_file_ref)
    ram_usage_empty, flash_usage_empty = calc_flash_ram(elf_file_empty)

    # calculate the difference between the two
    # to get the RAM and FLASH usage of the model + inference framework runtime
    ram_diff = ram_usage_ref - ram_usage_empty
    flash_diff = flash_usage_ref - flash_usage_empty

    return (ram_diff, flash_diff)