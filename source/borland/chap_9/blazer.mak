#
# Borland C++ IDE generated makefile
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCCDOS  = Bcc +BccDos.cfg
TLINK   = TLink
TLIB    = TLib
TASM    = Tasm
#
# IDE macros
#


#
# Options
#
IDE_LFLAGSDOS =  -LE:\BC45\LIB
IDE_BFLAGS =
LLATDOS_blazerdexe =  -c -Tde -LC:\BC45\LIB
RLATDOS_blazerdexe =
BLATDOS_blazerdexe =
CNIEAT_blazerdexe = -IC:\BC45\INCLUDE -D
LNIEAT_blazerdexe = -x
LEAT_blazerdexe = $(LLATDOS_blazerdexe)
REAT_blazerdexe = $(RLATDOS_blazerdexe)
BEAT_blazerdexe = $(BLATDOS_blazerdexe)
CLATDOS_blazerdc =  -v- -R-
LLATDOS_blazerdc =
RLATDOS_blazerdc =
BLATDOS_blazerdc =
CEAT_blazerdc = $(CEAT_blazerdexe) $(CLATDOS_blazerdc)
CNIEAT_blazerdc = -IC:\BC45\INCLUDE -D
LNIEAT_blazerdc = -x
LEAT_blazerdc = $(LEAT_blazerdexe) $(LLATDOS_blazerdc)
REAT_blazerdc = $(REAT_blazerdexe) $(RLATDOS_blazerdc)
BEAT_blazerdc = $(BEAT_blazerdexe) $(BLATDOS_blazerdc)

#
# Dependency List
#
Dep_blazer = \
	blazer.exe

blazer : BccDos.cfg $(Dep_blazer)
  echo MakeNode

Dep_blazerdexe = \
	..\chap_8\black8.obj\
	..\chap_6\black6.obj\
	..\chap_5\black5.obj\
	..\chap_4\black4.obj\
	..\chap_3\black3.obj\
	blazer.obj

blazer.exe : $(Dep_blazerdexe)
  $(TLINK)   @&&|
 /v $(IDE_LFLAGSDOS) $(LEAT_blazerdexe) $(LNIEAT_blazerdexe) +
C:\BC45\LIB\c0m.obj+
..\chap_8\black8.obj+
..\chap_6\black6.obj+
..\chap_5\black5.obj+
..\chap_4\black4.obj+
..\chap_3\black3.obj+
blazer.obj
$<,$*
C:\BC45\LIB\fp87.lib+
C:\BC45\LIB\mathm.lib+
C:\BC45\LIB\cm.lib

|

blazer.obj :  blazer.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_blazerdc) $(CNIEAT_blazerdc) -o$@ blazer.c
|

# Compiler configuration file
BccDos.cfg :
	Copy &&|
-W-
-R
-v
-vi
-H
-H=blazer.csm
-mm
-f287
| $@


