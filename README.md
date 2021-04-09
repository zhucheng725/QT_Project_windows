# QT_Project_windows

安装流程:<br>

软件安装：<br>

Vs2015（需要选择win10 sdk，不然无法过cmake编译）；<br>

Cmake（https://cmake.org/download/），下载的是cmake-3.20.1-windows-x86_64.zip，没有下载source，直接解压可以在bin下打开cmake_gui.exe；<br>

Opencv我下载的是在线安装:<br>
https://iweb.dl.sourceforge.net/project/opencvlibrary/4.5.0/opencv-4.5.0-vc14_vc15.exe<br>
但在opencv官网应该可以找到，或者https://opencv.org/releases/ 下载source进行编译也可以，也试过是可行的；<br>

QT：<br>
http://qt.mirror.constant.com/archive/qt/5.12/5.12.0/qt-opensource-windows-x86-5.12.0.exe<br>
可以在qt官网下载其他版本的，因为5.7以上才支持vs2015版本的，所以下载了一个比较新的版本即可，安装的时候选择qt的msvc2015即可。（之前弄过qt的minGW的版本，会出现下载的是64位的creator，但通过minGW编译的opencv是32位，导致无法使用，估摸是可以解决，但由于时间问题，改成vs2015编译，是可以的），还有个需要说明的是，QT4和QT5的界面语法有区别，c++在界面代码方面有区别，python方面在connect触发方面有区别；<br>

建议先安装vs2015，然后cmake编译opencv，然后再用vs编译debug和release版本的opencv，具体的可以参考：<br>
https://blog.csdn.net/weixin_42727069/article/details/104293186<br>
在用python编译出的工程可以用pyinstaller来生成exe文件。<br>

编译好opencv后在D:\procedure\opencv\build\install\x64\vc14\bin下main有相关的dll文件，说明编译好了。<br>
在环境变量方面可以添加：
```
D:\procedure\opencv\build\install\x64\vc14\lib
D:\procedure\qt\5.12.0\msvc2015_64\bin
D:\procedure\opencv\build\install\x64\vc14\bin
C:\Program Files (x86)\Windows Kits\10\Windows Performance Toolkit\ 
```

由于使用qt的编译器在windows是不自带的，需要自行安装，win10可以商城安装windbg，然后安装<br>
https://download.microsoft.com/download/4/2/2/42245968-6A79-4DA7-A5FB-08C0AD0AE661/windowssdk/winsdksetup.exe <br>
这个winsdk会安装相关编译环境，如cdb.exe <br>
在qt的选项-工具-debuggers中选择 <br>
C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe <br>
Qmake选择 <br>
D:\procedure\qt\5.12.0\msvc2015_64\bin\qmake.exe <br>
编译器选择 <br>
D:\procedure\vs\VC\vcvarsall.bat x86_amd64 <br>
虽然只有cdb路径是需要自己添加，其他都是自动的，在这里备注下。 <br>

在qt建立工程后，需要在.pro文件设置路径，具体可以参考如下： <br>

```
## debug mode:
#INCLUDEPATH += D:\procedure\opencv\build\install\include \
#               D:\procedure\opencv\build\install\include\opencv2
#LIBS += -LD:\procedure\opencv\build\install\x64\vc14\lib -lopencv_core450d -lopencv_calib3d450d -lopencv_imgcodecs450d -lopencv_highgui450d -lopencv_imgproc450d -lopencv_videoio450d -lopencv_dnn450d -lopencv_features2d450d -lopencv_flann450d -lopencv_gapi450d -lopencv_ml450d -lopencv_objdetect450d -lopencv_photo450d -lopencv_stitching450d -lopencv_video450d
#DEPENDPATH += D:\procedure\opencv\build\install\include

# release mode:
INCLUDEPATH += D:\procedure\opencv\build\install\include \
               D:\procedure\opencv\build\install\include\opencv2
LIBS += -LD:\procedure\opencv\build\install\x64\vc14\lib -lopencv_core450 -lopencv_calib3d450 -lopencv_imgcodecs450 -lopencv_highgui450 -lopencv_imgproc450 -lopencv_videoio450 -lopencv_dnn450 -lopencv_features2d450 -lopencv_flann450 -lopencv_gapi450 -lopencv_ml450 -lopencv_objdetect450 -lopencv_photo450 -lopencv_stitching450 -lopencv_video450
DEPENDPATH += D:\procedure\opencv\build\install\include
```

其中debug mode 是在debug模式下把注释解开，同时把release mode的代码注释。假如想release环境下编译，把release mode下面的代码注释解开， 把debug mode的代码注释。<br>

可以先试一试随便在ui文件中摆放几个按钮看是否debug进去生成悬浮框。<br>

然后编译好的release工程后，假如需要跨电脑（windows）即可使用，可以在电脑找到<br>
windeployqt.exe文件，我的文件在<br>
D:\procedure\qt\5.12.0\msvc2015_64\bin\windeployqt.exe <br>
所以打开cmd路径到上面的路径，然后执行命令： <br>
windeployqt.exe 你想要的执行文件.exe <br>
比如我的： <br>
```D:\procedure\qt\5.12.0\msvc2015_64\bin>windeployqt.exe D:\procedure\build-OpenCam-Desktop_Qt_5_12_0_MSVC2015_64bit-Release\release\OpenCam.exe ```
它会在你的release的文件夹里添加相关的dll和qt的plugin。假如它链接的dll有缺失如我在跨其他window电脑时候发现它帮我导入的opencv的dll好像有点问题（具体我也不清楚问题是为什么），我就直接把opencv的相关dll放到release文件下，这样就好了。<br>
跨电脑就可以直接用了。假如没有经过windeployqt.exe处理的话，将debug或者release的exe直接发送到桌面端使用会报错，当然你在编译环境的电脑中把debug或者release文件夹全部拷贝到桌面可以用，但不美观。经过windeployqt.exe处理的话，可以把exe文件直接发送到桌面端就可以用了。<br>
该工程路径：<br>
D:\procedure\OpenCam <br>
该工程release路径：<br>
D:\procedure\build-OpenCam-Desktop_Qt_5_12_0_MSVC2015_64bit-Release<br>
