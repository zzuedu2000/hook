# hook
linux kernel 5.6 hook实例代码

1、说明
代码参考如下文章中的代码，在linux5.6内核中适配，编译运行成功
https://blog.csdn.net/qq_21792169/article/details/84583275

2、需要对ko模块加签名

scripts/sign-file sha512 certs/signing_key.pem certs/signing_key.x509  HookDriver/HookDriver.ko

scripts/sign-file sha512 certs/signing_key.pem certs/signing_key.x509  HookHere/HookHere.ko

3、把text_poke_smp修改为text_poke，text_poke_smp函数在内核中已不存在

4、运行如下命令测试
insmod HookDriver/HookDriver.ko 
insmod HookHere/HookHere.ko 
./HookTest/test

运行结果如下：
[   21.468547] open is successd!
[   21.469912] hook conntrack herok
11
33
44
55


