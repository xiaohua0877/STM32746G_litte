helle world
::c:\users\lan>del /?
::删除一个或数个文件。
::del [/p] [/f] [/s] [/q] [/a[[:]attributes]] names
::erase [/p] [/f] [/s] [/q] [/a[[:]attributes]] names
::  names         指定一个或多个文件或者目录列表。
::                通配符可用来删除多个文件。
::                如果指定了一个目录，该目录中的所
::                有文件都会被删除。
::  /p            删除每一个文件之前提示确认。
::  /f            强制删除只读文件。
::  /s            删除所有子目录中的指定的文件。
::  /q            安静模式。删除全局通配符时，不要求确认
::  /a            根据属性选择要删除的文件
::  属性          r  只读文件                     s  系统文件
::                h  隐藏文件                     a  存档文件
::                i  无内容索引文件               l  重分析点
::                -  表示“否”的前缀
::如果命令扩展被启用，del 和 erase 更改如下:
::/s 开关的显示句法会颠倒，即只显示已经删除的文件，而不显示找不到的文件。

:: 1、:: 注释内容（第一个冒号后也可以跟任何一个非字母数字的字符）
:: 2、rem 注释内容（不能出现重定向符号和管道符号）
:: 3、echo 注释内容（不能出现重定向符号和管道符号）〉nul
:: 4、if not exist nul 注释内容（不能出现重定向符号和管道符号）
:: 5、:注释内容（注释文本不能与已有标签重名）
:: 6、%注释内容%（可以用作行间注释，不能出现重定向符号和管道符号）
:: 7、goto 标签 注释内容（可以用作说明goto的条件和执行内容）
:: 8、:标签 注释内容（可以用作标签下方段的执行内容）