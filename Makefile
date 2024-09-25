SHELL := /bin/bash

init: get_testdata
	git submodule update --init
	cd third_party/tinyengine && git checkout db6dde7
	cd ../../
	git submodule update --recursive

get_testdata:
	gdown https://drive.google.com/uc?id=1F634GcS3E5R_AwRsRZGR5auEYMt7D4mO -O third_party/test_data.zip
	unzip -o third_party/test_data.zip -d ./

clean: check_clean
	rm -rf logs/*
	rm -rf data_gen/*
	rm -rf third_party/*

check_clean:
	@echo "This will delete all generated benchmark data. Are you sure you want to continue? [y/N]"
	@echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ]

.PHONY: clean check_clean