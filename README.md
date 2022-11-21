# 卫星轨道3D仿真软件satellitesim
WSAD分别控制视角的前后左右移动，EQ分别控制上下移动，滚轮控制移动速度。  
这是第一视角漫游模式，后面可能会加常见的3D模型显示模式。  

## 依赖库
[raylib](https://gitee.com/xd15zhn/raylib)  
[simucpp](https://gitee.com/xd15zhn/simucpp)  

## 可执行文件运行说明
在`satellitesim.exe`同级目录下新建文件夹并重命名为`resources`，将附件中所有资源文件移到该文件夹中，然后运行exe文件。  
`资源文件列表`：
```
earth.png
satellite.mtl
satellite.obj
satellite.png
skybox.png
skyboxfs.glsl
skyboxvs.glsl
sphere.mtl
sphere.obj
sun.png
```
如果电脑里没有C++环境或者没安装mingw，可能会提示缺少`libgcc_s_sjlj-1.dll`或`libstdc++-6.dll`，或者是类似"无法定位程序输入点于动态链接库上"的问题，将附件中的这两个文件放到`satellitesim.exe`同级目录下，或直接放入`C:\Windows\System32`目录下即可。  
运行后，如果背景为纯黑或模型显示不正常，请再次检查`resources`和所有资源文件的名称是否严格正确。

# 准备
下载所有代码和依赖库
- <https://github.com/glfw/glfw/releases/latest>
- <https://gitee.com/xd15zhn/tracelog>
- <https://gitee.com/xd15zhn/raylib>
- <https://gitee.com/xd15zhn/simucpp>
- <https://gitee.com/xd15zhn/satellitesim>
- <https://gitee.com/xd15zhn/matplotlibcpp> (选装)
- <https://gitee.com/xd15zhn/zhnmat> (选装)

# 安装
如果最新版出现不兼容的问题，建议在文末的链接中下载兼容版本。

# 其他参考链接
相关安装包: https://pan.baidu.com/s/1aAsfMtUq8xDsXYiGu8K3Tg 提取码: 2nkd  
- [Ubuntu下搭建OpenGL开发环境](https://blog.csdn.net/l1216766050/article/details/102787618)
- [下载安装MinGW-w64详细步骤](https://blog.csdn.net/jjxcsdn/article/details/123058745)
- [地球、月球及其他星球星空贴图](http://h.shanse8.com/read.php?tid-47872-ordertype-desc-displayMode-1.html)
