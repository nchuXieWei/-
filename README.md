# ForBlog------AnalogFileSystem
使用请参照程序提示

程序首次运行会初始化“文件系统”，使其有一个根文件夹root，而后可以在该根文件夹下新增、删除文件（夹），并可以先序、后序输出“文件系统”中
已存在的所有文件（夹）。
程序退出时会将“文件系统”信息存储在同级目录下的FileSystem.xw文件中
若FileSystem.xw文件已存在，则运行程序时程序会自动从该文件中获取“文件系统”的信息并构建“文件系统”

程序主要技术点为：
树的内存存储、插入操作、删除操作、先序遍历、后序遍历的实现
以及将树存储至文件以及从文件中读取、重构树的实现
  
代码注释偏少，但是配合变量名阅读应无大碍

程序仅具备基本的健壮性,初步测试程序各功能并无异常


此程序可扩展为能真正新增、删除、修改文件（夹）的程序，基本思路为将程序主体修改为win32程序，然后借助win32API将Insert(),Delete()中模拟
新增、删除的操作改为真实新增、删除的操作，并为程序新增modify功能。
