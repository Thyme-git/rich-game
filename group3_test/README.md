# 测试说明

## 请进入test目录，使用./start_test.sh脚本一键运行测试！

**指令：python UnitTest.py [可执行文件位置] [TestCase文件夹位置]**

正常执行的情况下，脚本会在可执行文件目录下建立TestCase_Print文件夹，其中为所有的dump文件。

## 测试样例编写

每个测试样例的输入和输出分别为名字相同但后缀不同的.in和.out文件，如ps1.in和ps1.out。

.in文件内为对被测试程序的标准输入，每一行为一次输入（可以理解为每输入一行就有一次回车）

.out文件内为测试对应的标准输出，需要包含换行

测试原理为通过子进程调用被测试程序，通过标准输入输出进行通信，将标准输出与预期进行比较，完全一致则测试通过。

## 测试结果说明

1. [TestUnitTest] 为脚本自检测试，如果脚本运行正确，则该测试通过。
2. [Test<XXX>] 表示XXX测试的输出。
3. [TestUnitTest]Test finished! Total:2, Success:2, Fail:0  // 表示TestUnitTest结束，一共测试了2次，成功2次，失败0次。
4. [TestSet]Test failed! test_case_path:TestCase/SingleCommand/set, output_path:../bin/TestCase_Print/barrier/set_barrier2_r.out // 表示[TestSet]中出现失败测试，一并输出预期输出和实际输出的地址
5. [TestStep][Errno 2] No such file or directory: '../bin/TestCase_Print/step3_r.out' // 表示脚本找不到这个测试输出，可能为被测文件并未生成dump文件或生成位置不正确。
6. Running warn:[BrokenPipeError]. Other tasks not terminated! // Linux管道出问题了，其他测试不会停止。一般出现这个提示，测试仍然能成功进行，无视即可。
7. Path does not exist!" // 测试路径有问题，请检查脚本调用传参是否正确
8. 最末一行为测试进度条，除非测试进程卡住，不然进度条基本是一瞬间推满。

## 注意事项

UnitTest.py会自动编译./UnitTestExample.c文件并测试UnitTest本身是否正常工作，测试结束后会删除编译生成的./UnitTestExample

requirements.txt 为python运行所需库。