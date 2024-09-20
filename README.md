# tinyml-framework-benchmark
Automatically compare machine learning inference on cortex M4 microcontroller on a per-layer basis

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

- Connect the [NUCLEO-L4R5ZI](https://www.st.com/en/evaluation-tools/nucleo-l4r5zi.html) board via USB.

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