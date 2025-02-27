# 编译原理综合实验

## 文件结构

```
├── src.txt
├── out.exe
├── include
    ├── shared.h
    ├── Lexer.h
    ├── Parser.h
    ├── SemanticAnalyzer.h
├── src
    ├── main.c      //主函数
    ├── Lexer.c     //词法分析函数
    ├── Parser.c    //语法分析函数
    ├── SemanticAnalyzer.c //中间代码四元组生成函数
```

## 函数结构

主函数调用语法分析函数，语法分析函数调用语义分析函数和词法分析函数生成四元组


## 运行

将代码写在`src.txt`中，程序会从里面读取并进行中间代码生成

## 注意

```
语句串→语句 { ; 语句 } ;
语句→ 赋值语句 | 条件语句 | 循环语句
```
由于语法规则中,`if`和`while`语句也属于语句，在语句串中也要以`;`结尾（与c语言语法规则不同），所以在`src.txt`中，`if`和`while`语句的结尾也要添加`;`，测试时要记得添加添加`;`

---
<div style="margin-top: 20px; text-align: right; font-size: smaller;">
    <p>李泽扬 2025/1/8</p>
</div>
