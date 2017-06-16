REG ADD "HKCU\Software\Google\Chrome\NativeMessagingHosts\ar.com.craving.hasarfp" /ve /t REG_SZ /d "%~dp0manifest.json" /f

icacls "%~dp0node_modules\hasar-fp\command.json" /grant Everyone:F /T >nul 2>&1