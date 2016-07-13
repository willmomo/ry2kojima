-------------------------------------------------------------------------
The information in this article applies to:

 - Microsoft Win32 Application Programming Interface (API) included with:
   - Microsoft Windows 95
-------------------------------------------------------------------------

SUMMARY
=======

The Windows 95 shell allows an application to register an application
desktop toolbar that behaves with the same look and feel as the Windows 95
taskbar. The application desktop toolbar is always attached to one of the
outside edges of the screen and can cause the size of the desktop to be
reduced so that other applications do not overlap the application desktop
toolbar.

The APPBAR sample code demonstrates how to implement an application desktop
toolbar that is resizable, can attach to any side of the screen, and allows
itself to be hidden in the same way the Windows 95 taskbar automatically
hides.


MORE INFORMATION
================

The application desktop toolbar interface for Windows 95 provides an easy
way for an application to attach windows to the edge of the screen so that
they don't overlap the Windows 95 desktop or other applications. The
ability to do this is especially useful in an application-launching program
such as the Microsoft Office Manager (MOM) utility or in an application
that needs to provide status information to the user as it runs in the
background.

The application creates its window normally and then registers itself with
the system as an application desktop toolbar (appbar). Once registered as
an appbar, any time the window moves it must negotiate with the system for
screen space by sending the requested rectangle to the system as part of an
ABM_QUERYSETPOS message. The system then checks to see if any other appbars
are using that space and adjusts the rectangle requested so as not to
overlap. When the appbar is moved, the system resizes the desktop and moves
any currently running applications so they do not overlap the appbar.

REFERENCES
==========

For more information on the application desktop toolbar interface, please
see the Chapter "Extending the Windows 95 Shell : Application Desktop
Toolbars" in the "Programmer's Guide to Windows 95."  The guide is
available in the Win32 SDK Help file under "Guides."
