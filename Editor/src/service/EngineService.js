import net from 'net';
import fs from 'fs';
import path from 'path';
import { spawn } from 'child_process';
import { EventEmitter } from 'events';
import { remote } from 'electron';

class EngineService extends EventEmitter {
    constructor() {
        super();
    }

    boot() {
        console.log("Booting");
        const ls = spawn(path.join(remote.app.getAppPath(), 'native/EditorService.exe').replace('app.asar', 'app.asar.unpacked'), {
            cwd: __dirname,
            detached: false,
            /*shell: false,
            windowsHide: true*/
        });

        ls.stdout.on('data', (data) => {
            console.log(`stdout: ${data}`);
        });

        ls.stderr.on('data', (data) => {
            console.error(`stderr: ${data}`);
        });

        ls.on('close', (code) => {
            console.log(`child process exited with code ${code}`);
        });

        ls.on('error', (err) => {
            console.log(err);
        });
    }
}

export default new EngineService();