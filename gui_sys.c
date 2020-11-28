#include "gui_window.h"

/*********************************************************
 * System Calls
**********************************************************/
int sys_createwindow(void)
{
    int x, y, cx, cy;
    char * title;
    struct RGB * content;
    argint(0, &x);
    argint(1, &y);
    argint(2, &cx);
    argint(3, &cy);
    argstr(4, &title);
    int p;
    argint(5, &p);
    content = (RGB *) p;

    acquire(&guiKernelLock);
    //Add to the wndList

    int i;
    for (i = 0; i < MAX_WINDOW_COUNT; ++i)
    {
        if(wndInfoList[i].hwnd == -1)
        {
            wndInfoList[i].hwnd = i;
            setRect(&wndInfoList[i].wndTitleBar, x, y - UTITLE_HEIGHT, cx, UTITLE_HEIGHT);
            setRect(&wndInfoList[i].wndBody, x, y, cx, cy);
            wndInfoList[i].procPtr = proc;
            wndInfoList[i].wholeContent = content;
            wndInfoList[i].content = content + UTITLE_HEIGHT * cx;
            wndInfoList[i].title = title;
            drawWndTitleBar(i);
            initMsgQueue(&wndInfoList[i].msgQ);
            wndCount += 1;
            focusOnWindow(i);
            break;
       }
    }
    release(&guiKernelLock);
    return i;
}

int repainted = 0;
int sys_repaintwindow()
{
    if (repainted == 0) {
        initDesktop();
        repainted = 1;
    }
    int hwnd;
    argint(0, &hwnd);
    acquire(&guiKernelLock);
    if (hwnd == focus) {
        switchuvm(wndInfoList[focus].procPtr);
        WndInfo* wnd = &wndInfoList[focus];
        // printf(1, "titlebar:(%d, %d)\n", wnd->wndTitleBar.x, wnd->wndTitleBar.y);
        drawBitmapToScreen(screen_buf, wnd->wholeContent, (Point){wnd->wndTitleBar.x, wnd->wndTitleBar.y}, (Size){wnd->wndBody.h + wnd->wndTitleBar.h, wnd->wndBody.w});
        drawBitmapToScreen(screen, wnd->wholeContent, (Point){wnd->wndTitleBar.x, wnd->wndTitleBar.y}, (Size){wnd->wndBody.h + wnd->wndTitleBar.h, wnd->wndBody.w});
        if (proc == 0) {
            switchkvm();
        } else {
            switchuvm(proc);
        }
    } else {
        repaintAllWindow(hwnd);
    }
    release(&guiKernelLock);
   return 0;
}

int sys_settimer()
{
    int hwnd, interval;
    argint(0, &hwnd);
    argint(1, &interval);

    acquire(&guiKernelLock);

    timerInfo.intervalList[hwnd] = interval;
    timerInfo.countList[hwnd] = 0;
    release(&guiKernelLock);
    return 0;
}

int sys_getmessage()
{
    int hwnd, p;
    argint(0, &hwnd);
    argint(1, &p);
    message *msg = (message *) p;

    if (proc != wndInfoList[hwnd].procPtr)
        return 0;
    acquire(&guiKernelLock);
    int r = getMessageFromQueue(&wndInfoList[hwnd].msgQ, msg);
    release(&guiKernelLock);
    return r;
}


int sys_updatewindow()
{
    int hwnd, x, y, cx, cy;
    argint(0, &hwnd);
    argint(1, &x);
    argint(2, &y);
    argint(3, &cx);
    argint(4, &cy);

    acquire(&guiKernelLock);
    updateWindow(hwnd, x, y, cx, cy);

    release(&guiKernelLock);
    return 0;
}

int sys_destroywindow()
{
    int hwnd;
    argint(0, &hwnd);
    acquire(&guiKernelLock);
    wndInfoList[hwnd].hwnd = -1;
    timerInfo.countList[hwnd] = -1;
    wndCount -= 1;
    focusOnWindow(focusList[wndCount - 1]);
    release(&guiKernelLock);
    return 0;
}