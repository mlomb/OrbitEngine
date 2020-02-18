import { Socket } from 'net';
import fs from 'fs';
import path from 'path';
import { spawn } from 'child_process';
import { EventEmitter } from 'events';
import { remote } from 'electron';

import { isDevMode } from '../ApplicationContext';

class EngineService extends EventEmitter {
    constructor() {
        super();

        // Note: Only Windows for now
        // TODO: name should contain a hash to allow multiple instances of the engine
        const name = 'OrbitEngineEditorService';
        this.ipcPath = `\\\\.\\pipe\\${name}`;

        this.ipcService = new Socket();
        this.ipcService.on('connect', () => {
            console.log("Connected via IPC to the Engine Service");
            this.emit('connected');
        });
        this.ipcService.on('error', (err) => {
            console.log(err);
            this.emit('disconnected');
        });
        this.ipcService.on('data', this.onData.bind(this));
    }

    boot() {
        if(!isDevMode)
            this.spawnService();
        this.initConnection();
    }

    spawnService() {
        const ls = spawn(path.join(remote.app.getAppPath(), 'native/EditorService.exe').replace('app.asar', 'app.asar.unpacked'), {
            cwd: __dirname,
            detached: false
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

    initConnection() {
        this.ipcService.connect(this.ipcPath);
    }

    onData(data) {
        this.emit('frame', data);
        console.log("DATA", data.length);
    }
}

export default new EngineService();