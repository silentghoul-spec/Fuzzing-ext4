Fuzzing ext4 filesystem using Janus 
===========================================


### Overview
This Repository is part of Theory and Practice of System Security Course. We tried to implement paper [Fuzzing File Systems via Two-Dimensional Input Space Exploration (IEEE S&P 2019)](https://gts3.org/assets/papers/2019/xu:janus.pdf) on our system. This paper describes the use of janus for fuzzing file system in two dimensions. First dimension is fuzzing input images and second dimensions is fuzzing syscalls. Key feature of Janus is that it is build on top of AFL and unlike tradional OS fuzzer which used to restart VM to reload fresh copy of OS lib, it uses Linus Kernel Library(https://github.com/lkl) to reload fresh copy of OS lib.
According to authors of paper Janus has found around 100 unique crashes in mainstream file systems with 32 CVEs assigned so far.




### Tested Environment
- OS: Ubuntu 18.04 LTS
- clang 6.0.1 and llvm 6.0.1 built from source

### Preparation
- Compile ff-gcc (for instrumentation)
    - cd ff-gcc
    - make
- Compile the core fuzzing engine
    - cd core
    - make
- Compile lkl 4.17 (ported by Wen Xu et al)
    - cd lkl
    - For example, if you want to fuzz ext4, ./compile -t ext4 -c
    - there are three target applications generated
        - ./tools/lkl/ext4-fsfuzz          for fuzzing images only
        - ./tools/lkl/ext4-executor        for fuzzing file operations only
        - ./tools/lkl/ext4-combined        for fuzzing both (Janus)
- Compile image parser 
    - cd fs/ext4
    - make
    - two output generated
        - ext4_wrapper.so: AFL linked with this .so to compress and decompress an image
        - ext4_standalone: this is used to release image offline given a compressed image and the original seed image. If you use *online* mode, you can release a batch of compressed images in an efficient way for reproducing.
    - Check fs/[fs name]/README.md for how to build in detail!

- Seed images
    - samples/evaluation        the seed images we used for evaluation in the paper 
    - samples/fuzzing           some additional images that we use for finding bugs
    - Let's assume we use samples/evaluation/ext4.image here
        - Build the istat file for generating starting program 
            - cd istat
            - ./istat -i ../samples/evaluation/ext4.image -t ext4 -o ext4.istat
                - Usage: -i: seed image -t: file system type -o: output
            - Then we get the initial image status file: istat/ext4-00.istat

- Run fuzzer
    - We need a directory to store seed programs based on the initial image status
        - mkdir prog
    - Create seed programs
        - ./core/create_corpus istat/ext4.istat prog
            - Usage: create_corpus [istat file] [output dir]
        - To show readable C code of a serialized program
            - ./core/program_show prog/open_read0
    - Create the input directory and the output directory for Janus
        - mkdir input
        - mkdir output
        - ./core/afl-image-syscall/afl-fuzz -b ext4 -s fs/ext4/ext4_wrapper.so -e ./samples/evaluation/ext4-00.image -S ext4 -y prog -i input -o output -m none -u 2 -- ./lkl/tools/lkl/ext4-combined -t ext4 -p @@
            - -b: shared memory name for storing image (which should be distinct)
            - -s: fs (de)compressor
            - -e: seed image
            - -S: AFL argument (slave name) (which should be distinct)
                - No -M support
            - -y: the seed program directory
            - -i: AFL argument (input directory) (which should be distinct)
            - -o: AFL argument (output directory)
            - -u: #CPU
        
            
        - How to check a generated testcase (compressed image + serialized program)
            - ./utils/afl-parse -i ./samples/evaluation/ext4.image -t ext4 -f output/ext4/crashes/id:000000,sig:11,src:000680,op:havoc,rep:64 -o tmp
                - Usage: -i: seed image -t: file system type -f: generated test case -o: output name
            - it will generate tmp.img: the mutated image to be mounted
            - tmp.c.raw: the serialized program 
            - tmp.c: the compilable program performed on the mutated image (for reproducing on a real OS)
            - You can use tmp.img and tmp.c.raw to reproduce the bug by ext4-combined in LKL

    - If you only want to fuzz images (and run the fixed operations in LKL's fsfuzz.c):
        - ./core/afl-image/afl-fuzz -b ext4 -s fs/ext4/ext4_wrapper.so -e ./samples/evaluation/ext4.image -S ext4 -i input -o output -m none -u 2 -- ./lkl/tools/lkl/ext4-fsfuzz -t ext4
    - If you only want to fuzz file operations (which are performed always on the same seed image):
        - ./core/afl-syscall/afl-fuzz -k -S ext4 -i prog -o output -m none -u 2 -- ./lkl/tools/lkl/ext4-executor -t ext4 -i ./samples/evaluation/ext4.image -p @@
        - Here the starting program folder is just the input directory


