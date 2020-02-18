const { app, BrowserWindow } = require('electron');
const path = require('path')
const url = require('url')
const devMode = !app.isPackaged;

function getLocalURL(filename) {
    return devMode ? `http://localhost:8080/${filename}` : url.format({
        pathname: path.join(__dirname, filename),
        protocol: 'file:',
        slashes: true
    });
}

app.on('ready', () => {
    let loading = new BrowserWindow({
        show: false,
        frame: false,
        resizable: false,
        backgroundColor: "#212121",
        width: 300,
        height: 200
    });
    
    loading.once('show', () => {
        let window = new BrowserWindow({
            width: 1024,
            backgroundColor: "#212121",
            frame: false,
            show: false,
            webPreferences: {
                nodeIntegration: true
            },
            minWidth: 700,
            minHeight: 600
        });

        window.once('ready-to-show', () => {
            window.show();
            loading.hide();
            loading.close();
            loading.destroy();
            loading = null;
        });
        
        window.loadURL(getLocalURL('index.html'));
    });
    
    loading.once('ready-to-show', () => {
        loading.show();
    });

    loading.loadURL(getLocalURL('splash.html'));
});