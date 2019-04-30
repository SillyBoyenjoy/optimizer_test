# optimizer_test
Test for Gurobi &amp; Cplex
Cplex版本1.2.8，Gurobi版本：8.1

请在相应目录（如cplex_test/）下直接输入make run运行程序
可在Makefile中修改相应参数，包括：
  SETS：需要测试的测试集文件夹名称
  Threads：测试中的线程数
  Gap：mip收敛的精度
  Srceen：是否在屏幕上打印详细log
  Log：是否在文档中打印详细log
测试用的算例集放在主目录optimizer_test/中。
