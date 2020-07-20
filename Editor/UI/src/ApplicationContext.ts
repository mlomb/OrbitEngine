declare var __ENV__: any;

const devMode = __ENV__.NODE_ENV !== 'production';
const target = __ENV__.TARGET;

export const isDevMode = devMode;
export const isElectron = target === 'electron';

export function getElectronWindow() {
    if(!isElectron)
        return null;
    else
        return require('electron').remote.getCurrentWindow();
}

console.log('isDevMode', isDevMode);
console.log('isElectron', isElectron);
