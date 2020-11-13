# QAppKiller

QAppKiller allows your app to know if it should reduce operations and/or inhibit the end user to use your application.

This library was quickly written by me during an afternoon in which I was desperate to get one of my clients to pay me. The corporate group received the final application binary and someone thought that it was a good idea to try to avoid paying me for my 8-month-long work.

This is the result, Dick-corp is currently investing heavily on marketing, and they are publishing an application that stops working & displays a banner saying that they are a bunch of cheap bastards. Fuck them.

In the case that they pay me, I just need to update my `Clients.json` file & the application immediately starts working perfectly again (and the end users shall never see my fuck-you banner). 

## Usage

1. Integrate `QAppKiller` to your project by using the `include()` qmake function in your `*.pro` file.

2. Integrate the following code in your `main.cpp` file:

```cpp
#include <QAppKiller>
#include <QGuiApplication>

int main(int argc, char**argv)
{
   QGuiApplication app(argc, argv);
	
   QAppKiller appKiller;
   appKiller.setClient("YOUR_APP_ID");
   appKiller.setUrl("URL_TO_APPCAST_JSON_FILE");
	
   // QAppKiller shall now automatically download
   // the app cast file & decide if we should
   // stop the application from working...
	
   //
   // You can then use QAppKiller::suspended() to
   // know if your application should stop working
   // to screw your dick client.
   // 
   // QAppKiller can also be used with your QML
   // code :)
   //
	
   return app.exec();
}

```

## App cast JSON

This file allows us to know which apps should we "kill" or "suspend", it has the following format:

```json
{
   "enabled":true,
   "clients":[
      {
         "client":"app_id_1",
         "suspended":false
      },
      {
         "client":"app_id_2",
         "suspended":true
      },
      {
         "client":"app_id_3",
         "suspended":false
      }
   ]
}
```

**Note:** If `enabled` is set to false, `QAppKiller` shall ignore the value of `suspended`.

## License

This project is released under the terms & conditions of the MIT licence. For more information, check the [`LICENSE.md`](LICENSE.md) file.



 
