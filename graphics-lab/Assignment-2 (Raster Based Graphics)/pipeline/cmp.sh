op=$1
if [ -z $1 ]; then
    op=-q
fi

diff --strip-trailing-cr $op test-cases/1/stage1.txt output/1/stage1.txt
diff --strip-trailing-cr $op test-cases/1/stage2.txt output/1/stage2.txt
diff --strip-trailing-cr $op test-cases/1/stage3.txt output/1/stage3.txt
diff --strip-trailing-cr $op test-cases/1/z_buffer.txt output/1/z_buffer.txt

diff --strip-trailing-cr $op test-cases/2/stage1.txt output/2/stage1.txt
diff --strip-trailing-cr $op test-cases/2/stage2.txt output/2/stage2.txt
diff --strip-trailing-cr $op test-cases/2/stage3.txt output/2/stage3.txt
diff --strip-trailing-cr $op test-cases/2/z_buffer.txt output/2/z_buffer.txt

diff --strip-trailing-cr $op test-cases/3/stage1.txt output/3/stage1.txt
diff --strip-trailing-cr $op test-cases/3/stage2.txt output/3/stage2.txt
diff --strip-trailing-cr $op test-cases/3/stage3.txt output/3/stage3.txt
diff --strip-trailing-cr $op test-cases/3/z_buffer.txt output/3/z_buffer.txt

diff --strip-trailing-cr $op test-cases/4/stage1.txt output/4/stage1.txt
diff --strip-trailing-cr $op test-cases/4/stage2.txt output/4/stage2.txt
diff --strip-trailing-cr $op test-cases/4/stage3.txt output/4/stage3.txt
diff --strip-trailing-cr $op test-cases/4/z_buffer.txt output/4/z_buffer.txt
