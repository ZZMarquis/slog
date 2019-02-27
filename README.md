# slog
A very simple pure C log library

------------------------------

1. Supported C89
2. Supported Windows and Linux
3. Supported log stacktrace
4. thread-safe

5. Not asynchronous

------------------------------

一个非常简单的纯C日志库，做它的目的是因为工作中遇到过这样的一些需求：  

1. 只是一个很简单的小项目
2. 支持C89
3. 需要的时候可以打印调用堆栈

这个时候我就不太想用一些比较大而全的日志库了  
  
PS:不适用于需要长时间运行的、会产生大量日志的程序，因为该库至始至终都只是往第一次init时找到/创建的那个文件里写日志，日志量太大会导致文件过大。  
