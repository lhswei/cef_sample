/// link to: libcef_dll_wrapper.lib libcef.lib

#define _WIN32_WINNT 0x0500

#include "windows.h"
#include "boost/thread.hpp"
#include "include/cef_app.h"
#include "include/cef_client.h"

#define USE_EXPLICIT_LOOP
#define GLOBAL_STUFF

class OffscreenClient : public CefClient,
   public CefLifeSpanHandler
   , public CefRenderHandler
{
public:
   OffscreenClient();
   virtual ~OffscreenClient();

   virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
      return this;
   }

   virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
      return this;
   }

   void SaveBrowserImage(CefRefPtr<CefBrowser> browser, const std::wstring &filename, const void* buffer, int width, int height);
   virtual void OnPaint(CefRefPtr<CefBrowser> browser,
      PaintElementType type,
      const RectList& dirtyRects,
      const void* buffer,
      int width, int height) OVERRIDE;

   // CefLifeSpanHandler methods
   virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
   virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
   // virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

   CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }

   virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE { rect.Set(0, 0, 800, 600);  return true; };

   // private:
   // The child browser window
   CefRefPtr<CefBrowser> m_browser;

   // Include the default reference counting implementation.
   IMPLEMENT_REFCOUNTING(OffscreenClient);
   // Include the default locking implementation.
   // IMPLEMENT_LOCKING(OffscreenClient);
};

OffscreenClient::OffscreenClient()
{
}

OffscreenClient::~OffscreenClient()
{
}

void OffscreenClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
   //AutoLock lock_scope(this);
   // keep browser reference
   m_browser = browser;
}

#ifdef USE_EXPLICIT_LOOP
bool doLoop = true;
#endif

void OffscreenClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
   //AutoLock lock_scope(this);

   // quit message loop
#ifdef USE_EXPLICIT_LOOP
   doLoop = false;
#else
   CefQuitMessageLoop();
#endif

   m_browser = NULL;
}

/*
bool OffscreenClient::DoClose(CefRefPtr<CefBrowser> browser)
{ 
   // AutoLock lock_scope(this);
   browser->GetHost()->ParentWindowWillClose();
   return false; 
}
*/

void OffscreenClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{
   // AutoLock lock_scope(this);

   static int s = 1;
   std::wostringstream oss;
   oss << std::setw(4) << std::setfill(L'0') << s++ << L".bmp";
   SaveBrowserImage(browser, oss.str(), buffer, width, height);
}

void OffscreenClient::SaveBrowserImage(CefRefPtr<CefBrowser> browser, const std::wstring &filename, const void* buffer, int width, int height)
{
   // Populate the bitmap info header.
   BITMAPINFOHEADER info;
   info.biSize = sizeof(BITMAPINFOHEADER);
   info.biWidth = width;
   info.biHeight = -height;  // minus means top-down bitmap
   info.biPlanes = 1;
   info.biBitCount = 32;
   info.biCompression = BI_RGB;  // no compression
   info.biSizeImage = 0;
   info.biXPelsPerMeter = 1;
   info.biYPelsPerMeter = 1;
   info.biClrUsed = 0;
   info.biClrImportant = 0;

   // Create the bitmap and retrieve the bit buffer.
   HDC screen_dc = GetDC(NULL);
   void* bits = NULL;
   HBITMAP bitmap = CreateDIBSection(screen_dc, reinterpret_cast<BITMAPINFO*>(&info), DIB_RGB_COLORS, &bits, NULL, 0);
   ReleaseDC(NULL, screen_dc);

   // Read the image into the bit buffer.
   if (bitmap == NULL)
      return;

   memcpy(bits, buffer, width * height * 4);

   // Populate the bitmap file header.
   BITMAPFILEHEADER file;
   file.bfType = 0x4d42;
   file.bfSize = sizeof(BITMAPFILEHEADER);
   file.bfReserved1 = 0;
   file.bfReserved2 = 0;
   file.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

   // Write the bitmap to file.
   HANDLE file_handle = CreateFile(filename.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
   if (file_handle != INVALID_HANDLE_VALUE)
   {
      DWORD bytes_written = 0;
      WriteFile(file_handle, &file, sizeof(file), &bytes_written, 0);
      WriteFile(file_handle, &info, sizeof(info), &bytes_written, 0);
      WriteFile(file_handle, bits, width * height * 4, &bytes_written, 0);
      CloseHandle(file_handle);
   }

   DeleteObject(bitmap);
}

#ifdef GLOBAL_STUFF
CefWindowInfo info;
CefBrowserSettings browserSettings;
CefMainArgs args;
CefSettings settings;
CefRefPtr<OffscreenClient> offscreenClient;
#endif

void browser_thread(CefRefPtr<OffscreenClient> &client)
{

#ifndef GLOBAL_STUFF
   CefWindowInfo info;
   CefBrowserSettings browserSettings;
   CefMainArgs args;
   CefSettings settings;
#endif

   // handle of current console
   // HWND hWnd = GetConsoleWindow();
   HWND hWnd = nullptr;

   // use this as the UI/main thread
   settings.multi_threaded_message_loop = false;

   // initialize CEF
   CefInitialize(args, settings, CefRefPtr<CefApp>());

   // set as offscreen renderer
   info.SetAsOffScreen(hWnd);

   client = new OffscreenClient();

   // synchronous/blocking call
   // CefBrowserHost::CreateBrowserSync(info, static_cast<CefRefPtr<CefClient>>(client), "http://www.google.com", browserSettings, nullptr);
   CefBrowserHost::CreateBrowserSync(info, client.get(), "http://www.google.com", browserSettings, nullptr);

#ifdef USE_EXPLICIT_LOOP
   while(doLoop)
   {
      assert(doLoop);
      assert(client->m_browser);
      CefDoMessageLoopWork();
   }
   
   assert(!doLoop);

#else
   CefRunMessageLoop();
#endif

   assert(!client->m_browser);

   client = NULL;

   // shut down CEF
   CefShutdown();
}

int main()
{
#ifndef GLOBAL_STUFF
   CefRefPtr<OffscreenClient> offscreenClient;
#endif

   // create browser in another thread
   boost::thread thread = boost::thread(browser_thread, boost::ref(offscreenClient));

   // wait for client creation
   while (!offscreenClient)
      Sleep(100);

   // wait for browser to be ready
   while (!offscreenClient->GetBrowser())
      Sleep(100);

   offscreenClient->GetBrowser()->GetMainFrame()->LoadURL("www.youtube.com");
   Sleep(4000);

   offscreenClient->GetBrowser()->GetHost()->CloseBrowser(false);

   // wait for thread to complete
   thread.join();

   return 0;
}
