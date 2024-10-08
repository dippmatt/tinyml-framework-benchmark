# tinyml-framework-benchmark
Automatically compare machine learning inference on cortex M4 microcontroller on a per-layer basis. Currently includes the frameworks [Glow](https://www.nxp.com/design/design-center/software/eiq-ml-development-environment/eiq-inference-with-glow-nn:eIQ-Glow), [STM32Cube.AI](https://stm32ai.st.com/stm32-cube-ai/), [Tiny-Engine](https://github.com/mit-han-lab/tinyengine) & [TensorFlow Lite for Microcontrollers](https://github.com/tensorflow/tflite-micro).

### Automatic Plot generation after benchmarking

Goal of this repository is to first, run the benchmarks on supported microcontroller platforms (currently [STM32L4R5ZI](https://www.st.com/en/evaluation-tools/nucleo-l4r5zi.html)).
The results are then visualized including the following plots:

Total Inference time per model is converted into a LaTeX table for each tested framework configuration after running the benchmarks:

<div style="text-align: center;">
<img src=./docs/inference_time2.png width="600"/>
</div>

Compare two frameworks against each other, highlighting faster vs slower layer runtimes:


<div style="text-align: center;">
<img src=./docs/glow_vs_glow_quant.png width="600"/>
</div>

Display rescheduling of layers in the computational graph and speedup / slowdown per layer with "pseudo" Sankey diagrams:

<div style="text-align: center;">
<img src=./docs/tflite_vs_glow.png width="600"/>
</div>

Visualize RAM and Flash usage per model:

<div style="text-align: center;">
<img src=./docs/ram_flash.png width="600"/>
</div>


### Structure

- src:

Source code for python automation to run frameworks
Note: ST's Cube MX AI, tiny engine & glow frameworks are fully automated, while tflite micro framework (also based on Cube MX projects) is not supported in an automated fashion.

- test_data

Contains test vectors / tensors for each use case (anomaly detection, image classification, keyword spotting, visual wake words)
The test data aims to provide data for two purposes:
 - Data to run inference on. At least one sample per class is provided.
 - Representative data: Some frameworks perform post training quantisation on their own, using float32 data as input to quantize the model (e.g. glow).
 For that purpose more data samples are needed to calibrate quantisation data (offset, scaling). 
 These datasets are equal to the calibration datasets used to quantize the tflite models in mlperf tiny, for each use case respectivly.
 ~100 to ~1000 samples (depending on the use case) are used in mlperf tiny to calibrate the quantised tflite models. Those are found in <test_data> directory as well.
 
- third_party

Contains third party repositories (e.g. tiny_engine repo, Glow), needed to compile tflite models into C.

### How to run

#### Software requirements:

- **[NXP Glow Model Compiler](https://www.nxp.com/design/design-center/software/eiq-ml-development-environment/eiq-inference-with-glow-nn:eIQ-Glow)**. Use the default installation path at `/opt/nxp/Glow` or adapt the path for `glow_compiler` and `glow_profiler` in [config.json](./src/benchmark_automation/config.json) to your installation path.
Note: NXPs glow installer is compiled using Ubuntu 20.04. A workaround for other OS versions is installing Glow in a docker container and exposing the binaries from the container to the host system. After installing Glow in a Ubuntu-20.04 container, run the following on the host machine to mount the glow compiler in the host system:

```bash
sudo mkdir -p /opt/nxp/Glow/bin
cd docker
docker build -t glow-installer .
docker run -it -v /opt/nxp/Glow/bin:/opt/nxp/Glow/bin glow-installer
```

Run the following in the contaier and accept the license:
```
./eIQ_Glow_Linux64.deb.bin
```

The Glow binaries are then mounted on the host machine at `/opt/nxp/Glow/bin/`.

- **[STM32CubeIDE V1.9.0](https://www.st.com/en/development-tools/stm32cubeide.html)** to get the correct arm-none-eabi-gcc compiler.
Add the compiler directory to $PATH globally, e.g. to `/etc/environment` 

    The compiler is found for example at `/opt/st/stm32cubeide_1.9.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.linux64_1.0.0.202111181127/tools/bin`

- **[STM32CubeMX V6.8.0](https://www.st.com/en/development-tools/stm32cubemx.html)**. Using newer STM32CubeMX versions breaks the automation flow and you will have to deal with pop-up windows from CubeMX.

- The [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)

- Apt packages: 
    - tk 
    - gcc-arm-none-eabi


#### Run the benchmark automation scripts:

- Connect the [NUCLEO-L4R5ZI](https://www.st.com/en/evaluation-tools/nucleo-l4r5zi.html) board via USB.

- Make sure to have read & write permissions for the device.
One option for that is to set a rule in `/etc/udev/rules.d/`:
Be aware that this creates global read write permissions for that device.

Find the vendor ID of the ST-Link device: `sudo lsusb` should result in the following output:
```
...
Bus 001 Device 004: ID 0483:374b STMicroelectronics ST-LINK/V2.1
...
```

Now create a udev rule for the ST-Link device: 

```sh
sudo vim /etc/udev/rules.d/50-stlink.rules
```
Add the following to the file and set idVendor and idProduct to your IDs:
```
SUBSYSTEM=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="0666"
KERNEL=="ttyACM0", MODE="0666"
```

- gdown is required to download test data from Google drive: `pip3 install gdown`

- Create a python environment for each framework (st, tiny_engine, glow)

```
cd src/benchmark_automation
python3 -m venv venv
source venv/bin/activate
pyhton3 -m pip install -r requirements.txt
```
For st, glow, tflite, tiny_engine:
```
cd src/<framework>
python3 -m venv venv
source venv/bin/activate
pyhton3 -m pip install -r requirements.txt
```

- Download and install [Glow](https://www.nxp.com/design/design-center/software/eiq-ml-development-environment/eiq-inference-with-glow-nn:eIQ-Glow). The executables are expected to be located here:

    `third_party/nxp/Glow/bin/model-tuner`
    `third_party/nxp/Glow/bin/model-profiler`

- Download and install [STM32CubeMX
](https://www.st.com/en/development-tools/stm32cubemx.html). 
    
    [STM32Cube.AI](https://stm32ai.st.com/stm32-cube-ai/) is also required. The Code templates in this repository work with X-CUBE-AI v8.1.

- Download the test data and tinyengine repository using `make init`

- **Run the benchmarks**

```
cd src/
source benchmark_automation/venv/bin/activate
python3 benchmark_automation/main.py
```

### Structure of results

The logs (stdout, stderr) are save to [logs/](./logs).

All benchmark results will be saved to [data_gen/](./data_gen).

### Adding models or frameworks
The automation scripts generate permutations from a config file of available frameworks, framework options and models. The configuration file can be edited here: [config.json](src/benchmark_automation/config.json).

The configuration expects the following parameters for use cases (models):

- model_int: Path to the quantized model in tflite format.
- model_fp: Path to the floating point model in tflite format.
- representative_data_fp: A compressed numpy array as a calibration data set for model quantization (should be > 1000 samples). This file is used in Glow to quantize floating point models.
The shape of the array is expected to be (number of samples, model input shape), e.g. (100, 128, 128, 3).
- test_data_fp: A compressed numpy array with test inputs that will be used during inference. The array shape is the same as above, but the recommended number of samples is 10 to decrease runtime during benchmarking.

The configuration expects the following parameters for frameworks:

- workdir: A temporary directory that is created to store intermediate results.
- out_dir: A directory to store the results.
- cube_template: A C-Code template to run model inference.
- Any other properties / binary switches for optimizaiton / additional C-Code templates that are required for a specific framework. These additional properties will be passed on to the framework-specific automation srcips in `src/<framework>`.

General settings:

- repetitions: Number of repetitions per input vector. Default is 10.
- cube_programmer: Cube Programmer is required to flash the microcontroller.