# MyShell

프로그램 개요
: Linux Shell 구현

 가. 쉘(shell)이란?
 
 쉘(shell)은 커맨드라인을 해석하는 프로세스이다. 즉, 쉘은 표준 입력으로부터 커맨드라인을 읽고, 입력된 라인을 해석하여 해당하는 명령을 실행시킨다. 쉘의 가장 간단한 동작 방식은 명령을 표준 입력으로부터 읽어 들인 후, 명령을 실행시킬 자식 프로세스를 포크(fork) 하는 것이다.
 그 후 부모 프로세스는 다른 명령을 읽어 들이기 전에 자식 프로세스가 종료할 때까지 기다린다. 또한, 쉘은 백그라운드 프로세스(background process), 시그널 처리 외에 파이프라인(pipeline)과 재지향(redirection)을 처리하며 본 과제는 이러한 쉘의 일부 기능을 수행하는 마이쉘(testsh)을 구현하는 것에 그 초점을 두었다.

 나. 마이쉘(testsh)의 구현

 testsh은 하나의 프로그램으로써 동작하며 다음 기능을 구현하였다:
  - multiple commands separated by semocolons
  - history command
  - cd command
  - ls command
