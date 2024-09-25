from typing import Dict
from enum import Enum
from os import chdir
import shutil
import subprocess
from pathlib import Path
import re
from shared_scripts.color_print import print_in_color, Color
from flash_and_readback import flash_mcu

def use_framework_compile(workdir: Path, cube_mx: Path, stm32ai: Path, repetitions: int, cube_programmer: Path, model: Path, cube_project: Path, cube_project_validate: Path):
    """Uses MX cube application to benchmark the model on the MCU.
    """
    step_output = dict()

    # Create the directpry where cube MX will validate the project
    validate_directory = workdir / Path("validate")
    validate_directory.mkdir()

    # Create the directories where cube MX will generate the projects
    generate_main_prj_dir = workdir / Path("gen", "main")
    generate_main_prj_dir.mkdir(parents=True, exist_ok=True)
    generate_valitate_prj_dir = workdir / Path("gen", "validate")
    generate_valitate_prj_dir.mkdir(parents=True, exist_ok=True)
    generate_reference_prj_dir = workdir / Path("gen", "reference")
    generate_reference_prj_dir.mkdir(parents=True, exist_ok=True)

    # Create the directory where the network report will be saved
    network_output_directory = workdir / Path("network_output")
    network_output_directory.mkdir(parents=True, exist_ok=True)
    # "network_output" is the directory where the network report will be saved,
    # as well as the output tensors of the network. We already know the path.
    step_output["tensor_values"] = network_output_directory / Path("network_val_io.npz")

    # Modify and copy the input script for MX to generate the project
    mx_generate_script = workdir / Path("..", "misc", "gen_mx_prj.txt")
    
    # compile main project, all layer project and validation project
    
    compile_project(workdir, 
                    cube_mx,
                    cube_programmer,
                    stm32ai,
                    model,
                    step_output, 
                    repetitions, 
                    cube_project, 
                    mx_generate_script, 
                    generate_reference_prj_dir, 
                    compile_mode=CompileMode.REFERENCE, 
                    postfix="_all_layers")

    compile_project(workdir, 
                    cube_mx,
                    cube_programmer,
                    stm32ai,
                    model,
                    step_output, 
                    repetitions, 
                    cube_project, 
                    mx_generate_script, 
                    generate_main_prj_dir, 
                    compile_mode=CompileMode.BENCHMARK, 
                    postfix="")

    compile_project(workdir, 
                    cube_mx, 
                    cube_programmer,
                    stm32ai,
                    model,
                    step_output, 
                    repetitions, 
                    cube_project_validate, 
                    mx_generate_script, 
                    generate_valitate_prj_dir, 
                    compile_mode=CompileMode.VALIDATE, 
                    postfix="_validate",
                    validate_directory=validate_directory,
                    network_output_directory=network_output_directory)    
    
    
    return step_output


def compile_project(workdir: Path, 
                    cube_mx: Path, 
                    cube_programmer: Path,
                    stm32ai: Path,
                    model: Path,
                    step_output: Dict, 
                    repetitions: int, 
                    cube_project: Path, 
                    mx_generate_script: Path, 
                    generate_prj_dir: Path, 
                    compile_mode: Enum, 
                    postfix: str, 
                    validate_directory: Path = None,
                    network_output_directory: Path = None):
    
    # get the .ioc file by matching the .ioc file type
    ioc_files = cube_project.glob("*.ioc")
    first_ioc_file = next(ioc_files, None)

    #project_name = cube_project.stem
    project_name = Path(first_ioc_file).stem
    print_in_color(Color.BLUE, "project_name: " + project_name)
    
    # set the mx generate script from the template path to the workdir path
    mx_generate_script = set_generate_path(workdir, mx_generate_script, first_ioc_file, generate_prj_dir)
    
    # Generate the C project
    generate_command = f"{cube_mx} -q {mx_generate_script}"
    
    # Compile the C project
    make_dir = generate_prj_dir / Path(project_name)
    compile_cmd = f"make -C {make_dir}"

    if compile_mode == CompileMode.VALIDATE:
        # set optimisation based option parameters
        with open(first_ioc_file, "r") as f:
            ioc_lines = f.readlines()
        ioc_lines = ''.join(ioc_lines)
        if '=time' in ioc_lines:
            allocate_inputs_option = "-O time"
        elif '=ram' in ioc_lines:
            allocate_inputs_option = "-O ram"
        else:
            allocate_inputs_option = ""
        # print("allocate_inputs_option: ", allocate_inputs_option)
        # import sys;sys.exit(0)

        # Generate the C project
        generate_log = workdir / Path("generate_report.txt")
        with open(generate_log, "w") as outfile:
            result = subprocess.run(generate_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True)
            # Print the output to the terminal
            print(result.stdout, end='')
            outfile.write(result.stdout)
    else:
        subprocess.run(generate_command, shell=True)

    if compile_mode in [CompileMode.BENCHMARK, CompileMode.REFERENCE]:
        # Set number of iterations per perf test to the reps variable, 
        # the default value is 16, we overwrite it here. 
        # Maybe there is a way to configure that in the MX IDE, that setting wasn't found.
        aiSystemPerformance_c = generate_prj_dir / Path(project_name, "X-CUBE-AI", "App", "aiSystemPerformance.c")
        main_c = generate_prj_dir / Path(project_name, "Core", "Src", "main.c")

        if compile_mode == CompileMode.BENCHMARK:
            set_repetitions_and_observer_end(aiSystemPerformance_c, repetitions=1, observer=True)
            set_end_of_benchmark(main_c , repetitions)
        else:
            set_repetitions_and_observer_end(aiSystemPerformance_c, repetitions=1, observer=False)
            set_end_of_benchmark(main_c , None)

    subprocess.run(compile_cmd, shell=True)

    elf_file = make_dir / Path("build", project_name + ".elf")
    print(elf_file)
    assert elf_file.exists()
    step_output["cube_template" + postfix] = elf_file

    # validate the model
    if compile_mode == CompileMode.VALIDATE:
        flash_mcu(cube_programmer, elf_file)
        
        # analyze model
        analyze_command = f"{stm32ai} analyze \
                --name network -m {model} \
                --type tflite \
                --compression none \
                --verbosity 1 \
                --output {network_output_directory} \
                --allocate-inputs {allocate_inputs_option}\
                --series stm_series \
                --allocate-outputs"
        subprocess.run(analyze_command, shell=True)
        
        # Validate the input data
        input_data_path = workdir / Path("data.npz")
        validate_command = f"{stm32ai} validate \
                --name network -m {model} \
                --type tflite \
                --compression none \
                --verbosity 1 \
                --workspace {validate_directory} \
                --output {network_output_directory} \
                --allocate-inputs {allocate_inputs_option}\
                --allocate-outputs \
                --mode stm32 \
                --valinput {input_data_path} \
                --desc /dev/ttyACM0:115200"
        
        subprocess.run(validate_command, shell=True)

        network_analyze_report = network_output_directory / Path("network_analyze_report.txt")
        extract_ram_flash(workdir, generate_log, step_output)
        extract_layer_names(workdir, network_analyze_report, step_output)
        # Check if the validation was successful and we got the output tensors
        assert step_output["tensor_values"].exists()
   
    return

def extract_layer_names(workdir: Path, analyze_report: Path, step_output: dict):
    """Extracts flash & ram from the generate log.

    Info found in the network generate report:
    Flash usage, RAM usage in bytes: network only and network + framework (toolchain, runtime)
    """
    with open(analyze_report, "r") as f:
        report = f.readlines()
    
    # extract layer names
    layer_name_init_flag = False
    layer_name_counter = 0
    layer_names = []

    # Example for layer name extraction:
    
    # Number of operations per c-layer
    # ------- ------ ---------------- --------- ------------ --------- ---------- 
    # c_id    m_id   name (type)            #op         type    #param   sparsity 
    # ------- ------ ---------------- --------- ------------ --------- ---------- 
    # 0       0      gemm_0 (dense)      82,048   smul_s8_s8    82,048     0.0865 
    # 1       1      gemm_1 (dense)      16,512   smul_s8_s8    16,512     0.2378 
    # 2       2      gemm_2 (dense)      16,512   smul_s8_s8    16,512     0.2889 
    # 3       3      gemm_3 (dense)      16,512   smul_s8_s8    16,512     0.3058 
    # 4       4      gemm_4 (dense)       1,032   smul_s8_s8     1,032     0.1047 
    # 5       5      gemm_5 (dense)       1,152   smul_s8_s8     1,152     0.0321 
    # 6       6      gemm_6 (dense)      16,512   smul_s8_s8    16,512     0.0653 
    # 7       7      gemm_7 (dense)      16,512   smul_s8_s8    16,512     0.0411 
    # 8       8      gemm_8 (dense)      16,512   smul_s8_s8    16,512     0.0270 
    # 9       9      gemm_9 (dense)      82,560   smul_s8_s8    82,560     0.0158 
    # ------- ------ ---------------- --------- ------------ --------- ---------- 
    # total                             265,864                265,864     0.0921 
    
    for line in report: 
        # remove trailing newline
        line = line.strip()
        # Detect start of layer naming block
        if "Number of operations per c-layer" in line:
            layer_name_init_flag = True
        # Detect header
        if layer_name_init_flag and '-------' in line:
            layer_name_counter += 1
            continue
        # Extract layer names
        if layer_name_counter == 2:
            layer_name = line.split(' ')
            # remove empty strings
            layer_name = list(filter(None, layer_name))
            layer_name = layer_name[-6] + ' ' + layer_name[-5] + ' ' + layer_name[-3]
            layer_names.append(layer_name)
        if layer_name_counter == 3:
            layer_name_init_flag = False
            
    # save the results
    step_output["layer_list"] = layer_names
    
    return


def extract_ram_flash(workdir: Path, generate_log: Path, step_output: dict):
    """Extracts the network generate report from the log.

    Info found in the network generate report:
    Flash usage, RAM usage: network only and network + framework (toolchain, runtime)
    """
    with open(generate_log, "r") as f:
        lines = f.readlines()
    # flag to indicate that the network analyze report is being read
    extraction_start = False
    report = []
    for i, line in enumerate(lines):
        if "Exec/report summary (generate)" in line:
            extraction_start = True
        if extraction_start:
            report.append(line)
            if "Creating txt report file" in line:
                report_file = line.split("Creating txt report file ")[1].strip()
                break
    report_file = workdir / (Path(report_file).stem + ".txt")
    
    with open(report_file, "w") as f:
        f.writelines(report)
    
    # extract flash and ram usage, as well as layer names
    start_summery_flag = False

    # Example for flash and ram usage extraction:

    # Summary per memory device type
    # --------------------------------------------
    # .\device       FLASH      %     RAM       %
    # --------------------------------------------
    # RT total      15,253   5.3%   4,496   68.7%
    # --------------------------------------------
    # TOTAL        286,133          6,544
    # --------------------------------------------
    
    for line in report:
        if "Summary per memory device type" in line:
            start_summery_flag = True
        if start_summery_flag:
            # RT total is the calculated ram and flash ONLY of the runtime using 
            # text, rodata, data and bss sections of the elf file
            if "RT total" in line:
                line = line.split("RT total")[1]
                result = re.split(r'\s+', line.strip())
                flash_usage_rt = result[0].replace(",", "")
                ram_usage_rt = result[2].replace(",", "")
            # TOTAL is the calculated ram and flash of the runtime AND the network (weights, activations, io buffers)
            if "TOTAL" in line:
                line = line.split("TOTAL")[1]
                result = re.split(r'\s+', line.strip())
                flash_usage = result[0].replace(",", "")
                ram_usage = result[1].replace(",", "")
                break
    
    # save the results for ram and flash in bytes 
    step_output["flash"] = int(flash_usage)
    step_output["ram"] = int(ram_usage)
    step_output["flash_rt"] = int(flash_usage_rt)
    step_output["ram_rt"] = int(ram_usage_rt)
    
    return


def set_repetitions_and_observer_end(aiSystemPerformance_c: Path, repetitions: int, observer: bool = True):
    """Sets the number of repetitions for a given performance test 
    and if per-layer measurements should be used.
    
    Args:
        aiSystemPerformance_c (Path): Path to the C file to modify
        num_reps (int): number of repetitions to set
    """
    pattern = r"#define _APP_ITER_[ \t]+\d+"
    with open(aiSystemPerformance_c, "r") as f:
        lines = f.readlines()
    for i, line in enumerate(lines):
        # set number of repetitions
        if re.match(pattern, line):
            lines[i] = f"#define _APP_ITER_ {repetitions}\n"
        # set termination condition of benchmark
        if "idx = (idx+1) % AI_MNETWORK_NUMBER;" in line:
            lines[i+1] = "    r=CONS_EVT_QUIT;\n"
        # set observer
        if not observer:
            if "#define USE_OBSERVER" in line:
                lines[i] = "#define USE_OBSERVER 0\n"
    with open(aiSystemPerformance_c, "w") as f:
        f.writelines(lines)
    return


def set_end_of_benchmark(main_c: Path, repetitions: int):
    """Sets the termination condition for the benchmark in the main() functions's while loop.
    """
    found_line_flag = False
    with open(main_c, "r") as f:
        lines = f.readlines()
    for i, line in enumerate(lines):
        if "MX_X_CUBE_AI_Process();" in line:
            found_line_flag = True
            continue
        # set repetitions for "BENCHMARK" mode, repeat each measurement 'repetitions' times
        if found_line_flag and repetitions is not None:
            insertion_before = f'  int k;\n  uint8_t repetition_message[] = "Finished repetition!\\r\\n";\n  for (k = 0; k < {repetitions}; k++){{'
            insertion_after = 'HAL_UART_Transmit (&hlpuart1, repetition_message, sizeof (repetition_message), HAL_MAX_DELAY);\n  }\n  uint8_t end_message[] = "Finished timing measurements!\\r\\n";  HAL_UART_Transmit (&hlpuart1, end_message, sizeof (end_message), HAL_MAX_DELAY);  return 0;'
            lines[i-2] = insertion_before
            lines[i] = insertion_after
            break
        # omit repetitions for "REFERENCE" mode for now, because data aquisition is not updated yet, delete this if implemented later
        if found_line_flag and repetitions is None:
            insertion = '  uint8_t end_message[] = "Finished timing measurements!\\r\\n";\n  HAL_UART_Transmit (&hlpuart1, end_message, sizeof (end_message), HAL_MAX_DELAY);\n  return 0;\n'
            lines[i] = insertion
            break
    with open(main_c, "w") as f:
        f.writelines(lines)
    return


def set_generate_path(workdir: Path, mx_generate_script: Path, ioc_file: Path, cube_prj_gen: Path):
    """Reads the MX generate script and sets the path to the generated project.
    Then saves the script to workdir.
    """
    with open(mx_generate_script, "r") as f:
        lines = f.readlines()
    for i, line in enumerate(lines):
        if "<CUBE_PRJ_IOC>" in line:
            line = line.replace("<CUBE_PRJ_IOC>", str(ioc_file))
            lines[i] = line
        elif "<CUBE_PRJ_GEN>" in line:
            line = line.replace("<CUBE_PRJ_GEN>", str(cube_prj_gen))
            lines[i] = line
    save_path = workdir / Path("gen_mx_prj.txt")
    with open(save_path, "w") as f:
        f.writelines(lines)
    return save_path
    

class CompileMode(Enum):
    BENCHMARK = 1
    REFERENCE = 2
    VALIDATE = 3


if __name__ == "__main__":
    from pathlib import Path
    
    # Prepare directory structure
    workdir = Path.cwd() / Path("..", "workdir")
    submodule_dir = Path.cwd() / Path("..", "..", "..", "third_party", "tinyengine")
    
    model = Path("/home/matthias/Documents/BA/results/models_and_data/anomaly_detection/trained_models/ad01_int8.tflite")
    prep_tinyengine_framework(workdir, submodule_dir, model)
