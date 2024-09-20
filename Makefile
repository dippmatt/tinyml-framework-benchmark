init: get_testdata get_tinyengine
	git submodule update --init
	cd third_party/tinyengine && git checkout db6dde7
	cd ../../
	git submodule update --recursive
	
get_tinyengine:
	git submodule update --init
	cd third_party/tinyengine && git checkout db6dde7
	cd ../../
	git submodule update --recursive

get_testdata:
	gdown https://drive.google.com/uc?id=1F634GcS3E5R_AwRsRZGR5auEYMt7D4mO -O third_party/test_data.zip
	unzip -o third_party/test_data.zip -d ./

clean:
	rm -rf third_party/*