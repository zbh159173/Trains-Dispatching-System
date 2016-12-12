#================================================================================#
#==============================1709 Train System=================================#

How to use it?

(1). Input [A/B/C] [P/T/S/+/-]|[1/2/3/4] [1/2/3/4] or [Q] as an example.
1.AP BP CP
  Train A/B/C will pause.
2.AT BT CT
  Train A/B/C will change its direction.
  If the original direction is CCW(counter-clockwise), it will change to CW(clockwise) and vice versa.
3.AS BS CS
  Train A/B/C will change its service strategy.
  If the original direction is FAFS(first-arrived-first-serve), it will change to BTW(by-the-way) and vice versa.
4.A+ A- B+ B- C+ C-
  Train A/B/C will speed up/down.
  Maximum is 5 and minimum is 0.
5.A14 B23 C31, etc.
  Give the station on the railway same as the train A/B/C a new task. The task will be released by station[1/2/3/4] to station[1/2/3/4].
6.Q
  Save information and exit in 5 sec.
7.Attention! What you input does NOT always show on the screen. So if you input 'A' and then input '+' 2 sec later,
  it will do what 'A+' will do.
  But, if you input 'BA+', it will be regarded as an invalid command.
8.If you input an invalid command, you can see an error message.
9.If a train's FIRECOIN number is equals to or greater than 5, it will run automatically.
(2). The information will show on the screen and in "Result.txt".
(3). "Train.exe", "railway*.txt", "train*.txt", "task.txt" should be in the same folder.


Thanks for your using.