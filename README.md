# Preface

之前就听说了有剧组来咱们学校拍戏了，最近小红书也开始刷到了。其中那个用“C语言”写的爱心很吸引我，我寻思我能不能做一个出来呢？

画画的东西去年小圣诞树已经做过了 (而且C写出这个效果，已经远远超出我对这个语言的理解了)，正好 inspired by [donut.c](https://www.a1k0n.net/2011/07/20/donut-math.html)，可以换个法子来玩

 (原文的操作更加高阶，故用到的知识也牛一点，我这里东西比较简单，只要*一点点*的**微积分**知识就可以了)
 
> 朋友们，这个不土！！爱心只是个表现形式！你看我取名也叫 `math.c`，重点是它告诉了我们为什么要学好数学😭 (好吧，其实取这个名字也是 inspired by [Joma](https://www.youtube.com/watch?v=sW9npZVpiMI)) 

~~费劲手敲，重复造轮子，不务正业，有啥意义。别骂了别骂了😭~~

## Quick Look

![QuickLook](QuickLook.gif)

# 方法

## 浓缩一下

反正是在二维平面上做图，理所应当想到就用坐标轴表示。所以要找一个爱心方程

对于爱心的缩放可以用变换，我就使用了伸缩变换。而这个变换的位移曲线可以自己取，我并没有掌握多少漂亮的位移曲线，所以就用线性的了 (有一个例外) 

对于爱心周围的散点，可以考虑延曲线上各点法线方向上延伸。延伸的位移曲线也可以个性化

对于画面的明暗度，可以用 `.,-~:;=!*#$@` 这些 ASCII 字符表示。copy from [donut.c](https://www.a1k0n.net/2011/07/20/donut-math.html)

## 详细过程

那么就要先找一个爱心的方程，我随便在百度上找了一个： (后面的+2是我加上去的，因为我觉得这个爱心不够居中) 

$$
f(x,y)=
\begin{cases}
x=16\sin ^{3} t \\
y=13\cos t-5\cos(2t)-2\cos(3t)-\cos(4t)+2
\end{cases}
$$

求曲线上点的法线其实最终都是要找点，那么找法向量是最好的。由于平面曲线的切线的斜率就是 $\displaystyle{\frac{\text dy}{\text dx}}$

$$
\therefore \vec T=(\frac{\text dy}{\text dx}, 1)
$$

$$
\begin{align}
\therefore \vec n= & (1,-\frac{\text dy}{\text dx}) \\
= & (1,-\frac{\text dy}{\text dt}\frac{\text dt}{\text dx}) \\
= & (\frac{\text dx}{\text dt},-\frac{\text dy}{\text dt})
\end{align}
$$

单位化之后配合位移曲线控制散点的运动风格

确定法向量的方向，可以通过与曲线上的点作为向量点积，>0 则方向朝外，<0 则方向朝内

## 注释

`line 95` 特判。 爱心上半部分向下的尖尖在实际函数中是两个|斜率|很大且非常靠近的曲线，所以朝外的散点不作处理的话，会越过另一根曲线进入爱心内部

`line 143` 特判。同理，爱心下半部分的尖尖也是这样，不作处理的话，向内的散点会越过到外部

---

详细过程也不详细，没事，慢慢加 (有没有人看还是个问题🤔) 
