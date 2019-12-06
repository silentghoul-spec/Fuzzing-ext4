Command line used to find this crash:

./core/afl-image-syscall/afl-fuzz -b ext4 -s fs/ext4/ext4_wrapper.so -e ./samples/evaluation/ext4.img -S ext4 -y prog -i input -o output -m none -u 2 -- ./lkl/tools/lkl/ext4-combined -t ext4 -p @@

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 0 B.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please drop
me a mail at <lcamtuf@coredump.cx> once the issues are fixed - I'd love to
add your finds to the gallery at:

  http://lcamtuf.coredump.cx/afl/

Thanks :-)
