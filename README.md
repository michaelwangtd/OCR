# OCR
An optical character recognition(OCR) system,mainly to recognize printed letters and numbers,using c++ implementation.

* file structure：
```
----main
    ----process
        ----controller
            ----denoiselocationsegment
                ----digitalimgpro
        ----alg
        ----camera
        ----utils
        ----test
```

* process：
```
    数字图像处理
        摄像头获取图像
        去噪处理
            中值滤波
            灰度处理
            自适应二值
            形态学
        目标区域裁剪
            （
            锚点跟踪
            目标区域定位
            ）
        倾斜校正
            霍夫变换
                霍夫线检测
                倾斜角计算
                图像旋转
        字符分割
            垂直投影
        字符块归一化
    算法
        KNN
        （
        SVM
        LeNet
        VGG
        ResNet
        Inecption
        ）
```

* version
```
    vs2015
    opencv 3.4.0
```