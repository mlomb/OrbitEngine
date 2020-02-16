const devMode = process.env.NODE_ENV !== 'production';
const target = process.env.TARGET;

export function isDevMode() {
    return devMode;
}

export function isElectron() {
    return target === 'electron';
}

export function getElectronWindow() {
    if(!isElectron())
        return null;
    else
        return require('electron').remote.getCurrentWindow();
}
