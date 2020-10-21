
Aidan Martin, Eliot Shea, and Dylan McClure

File Listing:
Part1
empty.c
part1.c
empty.trace
part1.trace
Part2
Makefile
my_timer.c
Part3
Makefile
elevator.c
Makefile Descriptions:
We create the object code file and set the kernel directory
using "shell uname -r" for the build and store the current directory.
The modules are compiled and built with signals for the directories.
All files produced from making can be removed using "make clean".

Division of Labor:
Part1: Aidan/Dylan
Part2: Eliot/Dylan
Part3: Aidan/Eliot
- step 1: Kernel Module- Aidan/Eliot
- step 2: System Calls- Eliot
- step 3: /Proc- Aidan
- step 4: Test- all

GIT Commit Log:
Merge: 7fd1005 212642f
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Tue Oct 20 14:43:46 2020 -0400

    Merge branch 'master' of https://github.com/am16br/COP4610-Project-2

commit 7fd10059196af698909fdbec5741b05848de2425
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Tue Oct 20 14:42:46 2020 -0400

    changed increment elevator to prevent floor 0

commit 212642f9cbcb1c79ae72d23aa551111c669cc770
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Mon Oct 19 18:37:22 2020 -0400

    Update README.md

commit 73f763e3b92edb706fbb1b5c76d6395faa1cfba7
Merge: b2c11cf 4c22e3e
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Mon Oct 19 18:32:27 2020 -0400

    Merge branch 'master' of https://github.com/am16br/COP4610-Project-2

commit 4c22e3e3f4648f8ba1c07e8fd51385e6df80c6a3
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Mon Oct 19 18:30:42 2020 -0400

    Delete wrappers.h

commit bbb4d8e9ca14bb89b5a8d3d3aea5e7bc669234cf
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Mon Oct 19 18:30:35 2020 -0400

    Delete sample_proc.txt

commit 83e4fb89532bb0cc0240a5dcb258a03fd9849882
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Mon Oct 19 18:30:29 2020 -0400

    Delete producer.c

commit 8eb3550aed4132bad82fd6bbc347f233510fd98e
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Mon Oct 19 18:30:22 2020 -0400

    Delete consumer.c

commit bc3e1ca73426492c58daf6de1bbd7f7a9df56b46
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Mon Oct 19 18:30:14 2020 -0400

    Delete Makefile

commit b2c11cf5ad2be0d633ed43593eb779d2b38a5d02
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Mon Oct 19 18:26:53 2020 -0400

    Added timer, doesn't print to procfile, only printk

commit 30a579bcae63a07ea169db2d3a4d577140b50ba5
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Sun Oct 18 23:15:33 2020 -0400

    Aidan final

commit 71748678f3b4c227e88480bdf5a696b5881d1daf
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Sun Oct 18 23:14:21 2020 -0400

    Eliot's code (my bad should have committed from desktop)

commit bbe742bd0db2fc57a0610778204e3a32bb61bffd
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Sun Oct 18 23:07:48 2020 -0400

    FULLY FUNCTIONAL

commit b8d0498c8c64312210cbcd5b7e5f84fa03ecf0b1
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Sun Oct 18 22:47:18 2020 -0400

    Debugged elevator functionality

commit c6315abbe1d9e26ed3fb3d76417be9da0d596f03
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Sun Oct 18 17:56:01 2020 -0400

    add upload, update load

commit 282b52ce8cf5841e49226aefc09ea73878620b17
Merge: 8c6c2fe 03c1ead
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Sun Oct 18 17:16:12 2020 -0400

    Merge branch 'master' of https://github.com/am16br/COP4610-Project-2

commit 8c6c2fe24742cf57632d3ba062791674f5618a90
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Sun Oct 18 17:13:03 2020 -0400

    Merged changes and added thread prototype

commit 03c1ead349bbc8687477b15f63edc64b6eee3184
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Sun Oct 18 16:44:43 2020 -0400

    proc functional

commit 7d9f0ec0b4c2489de61ff99eb81baa87d25715d1
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Sun Oct 18 16:25:29 2020 -0400

    Added elevator thread prototype

commit 0a30fb93c05975fd162ba809867dea15bc3ab654
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Sun Oct 18 12:46:46 2020 -0400

    Update elevator.c

commit 9db4b3d58504a036b6425f0deb7a460539c38e72
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Thu Oct 15 20:14:23 2020 -0400

    request fix

commit 3c3dee7affadea0d3359e3f036cc959c5136c53d
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Thu Oct 15 17:49:55 2020 -0400

    added elevator_run

commit 56c591aa52b0da3785969423c45adf6f2992cfef
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Thu Oct 15 17:38:15 2020 -0400

    quick fix

commit b36e96ac5822b7d77e89ff2d815ddc9dfb44c7f7
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Thu Oct 15 15:28:31 2020 -0400

    updated printfloors()

commit 2343537c2c12a7e978b5b4ddec9dafa510396371
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Thu Oct 15 12:33:55 2020 -0400

    Fixed profile, isolated issues to printfloors()

commit 0819cb39890b169388a58947993f094f6d260f13
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Wed Oct 14 14:01:53 2020 -0400

    Fixed code for proc file, successfully compiles

commit 14ce2fcc06583c9819fa72b24787b4515058ca89
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Tue Oct 13 12:19:06 2020 -0400

    Added locking to issue_request

commit 8a94791b2027765d63ebdc07414cb2abe4648c5a
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Tue Oct 13 11:56:39 2020 -0400

    Implemented issue_request, and included the TA's user space program to test our file

commit 0e5ed929a01b2b2e8ecba44ce2cf5e00c060394c
Author: Eliot Shea <eliot.shea@gmail.com>
Date:   Tue Oct 13 10:49:53 2020 -0400

    Renamed Part 3 to Part3 to fix Makefile errors

commit 2d5fb1158c4e1c93beb73ff857e5ed0cfe090c8d
Author: Eliot Shea <45671399+eliotshea@users.noreply.github.com>
Date:   Tue Oct 6 14:37:12 2020 -0400

    Added files to patch kernel with new syscalls

commit 3d487d40e400111740af1f2355ceb18badbcbaa6
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Tue Oct 6 14:01:26 2020 -0400

    Update elevator.c

commit c90fd2bf61af7f897314d8365b0c88a625fdffae
Author: am16br <am16br@my.fsu.edu>
Date:   Mon Oct 5 23:21:53 2020 -0400

    Initial Part 3 (work in progress)

    Modified the Threading with Blocking example to handle the proc entry and elevator states. I think its a good starting point but needs some chnges before running. Need to fix lists for passengers and waiting, and add to run function

commit 9416e4cc985960d2544241d1c0d4940655497213
Author: am16br <am16br@my.fsu.edu>
Date:   Thu Oct 1 16:04:48 2020 -0400

    part 2 initial

commit 35b25cfc8887c858fbc8557da4a0ff8b3b7eeb66
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Wed Sep 16 14:05:12 2020 -0400

    Add files via upload

commit 042a264850c9e4188539ca6bfb54477907ff4a57
Author: am16br <65733333+am16br@users.noreply.github.com>
Date:   Wed Sep 16 13:17:26 2020 -0400

    Initial commit
