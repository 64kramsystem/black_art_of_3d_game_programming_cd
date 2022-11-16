                    
         BLACK ART OF 3D GAME PROGRAMMING: CD-ROM Installation Notes

To install the contents of the CD-ROM onto your computer, follow these steps:

1. Place the CD into your CD-ROM drive.
2. Make the CD-ROM drive the current drive by typing E: or F: (or whatever 
   the CD-ROM drive letter is).
3. Type in INSTALL.EXE at the DOS prompt. This will launch the installer 
   program.
4. Once in the installation program, follow the on-line help and 
   instructions.

IMPORTANT: Please note that the installation program installs whole 
directories and not individual files. If you wish to replace a file, you
will have to copy it using the DOS COPY command or Windows File Manager.
 
If you have trouble installing any portion of the CD-ROM to your hard drive, 
use either the DOS COPY or XCOPY command to manually copy the files from the 
CD to your hard drive. If you do not use the installer program and copy the 
files instead, be sure to change the Read Only attribute on all the files 
you copy.

To "copy" a single fileand change it'sread-only attribute:

D:\[directory name]>copy [filename] C:\[directory name] -r


to copy multiple files and change their read-only attributes:


D:\[directory name]>xcopy *.* C:\[directory name] -r


To change the Read Only attribute using Windows File Manager.
 
Highlight the file, press ALT-ENTER. When the dialogue box comes up de-select
the read-only attribute, press enter, or click okay.

