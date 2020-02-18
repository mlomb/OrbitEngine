import React from 'react';
import * as twgl from 'twgl.js';

import EngineService from '@service/EngineService';

const vs = `
attribute vec4 a_position;
varying vec2 v_texcoord;

void main() {
    gl_Position = a_position;
    v_texcoord = a_position.xy * 0.5 + 0.5;
}
`;

const fs = `
precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D u_sampler;
void main() {
    gl_FragColor = texture2D(u_sampler, v_texcoord);
}
`;

const verts = [
     1,  1,
    -1,  1,
    -1, -1,
     1,  1,
    -1, -1,
     1, -1,
];

export default class RenderOutput extends React.Component {

    constructor(props) {
        super(props);

        this.canvasRef = React.createRef();
        this.onFrame_func = this.onFrame.bind(this);
        this.frame_func = this.frame.bind(this);
        this.rAF = null; // request ID from requestAnimationFrame
        this.last_frame = null; // most up to date
    }

    componentDidMount() {
        const canvas = this.canvasRef.current;
        let gl = this.gl = canvas.getContext("webgl");
        let program = this.program = twgl.createProgramFromSources(gl, [vs, fs], ["a_position"]);

        gl.useProgram(program);

        let vertBuffer = this.vertBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vertBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verts), gl.STATIC_DRAW);
        gl.enableVertexAttribArray(0);
        gl.vertexAttribPointer(0, 2, gl.FLOAT, false, 0, 0);

        let tex = this.tex = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, tex);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

        // subscribe to frame updates
        EngineService.on('frame', this.onFrame_func);

        let ro = new ResizeObserver(this.resized.bind(this));
        ro.observe(canvas.parentElement);
    }

    onFrame(frame) {
        if(this.rAF !== null) {
            // frame skipped
        }
        this.last_frame = frame;
        this.requestFrame();
    }

    requestFrame() {
        if(this.rAF === null) {
            this.rAF = requestAnimationFrame(this.frame_func);
        }
    }

    frame() {
        this.rAF = null;

        const { gl } = this;
        const canvas = this.canvasRef.current;

        //console.log(this.last_frame.length, 3 * canvas.width * canvas.height);
        
        //let frame = this.last_frame.slice(0, 3 * canvas.width * canvas.height);
        //this.last_frame = null;

        gl.bindTexture(gl.TEXTURE_2D, this.tex);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, canvas.width, canvas.height, 0, gl.RGB, gl.UNSIGNED_BYTE, this.last_frame);
        gl.drawArrays(gl.TRIANGLES, 0, 6);
    }

    resized() {
        const canvas = this.canvasRef.current;

        canvas.width = canvas.parentElement.clientWidth;
        canvas.height = canvas.parentElement.clientHeight;

        this.requestFrame();
    }

    componentWillUnmount() {
        EngineService.off('frame', this.onFrame_func);

        if(this.rAF) {
            cancelAnimationFrame(this.rAF);
            this.rAF = null;
        }

        const { gl } = this;

        gl.useProgram(null);
        gl.bindTexture(gl.TEXTURE_2D, null);

        gl.deleteTexture(this.tex);
        gl.deleteBuffer(this.vertBuffer);
        gl.deleteProgram(this.program);
        
        this.tex = undefined;
        this.vertBuffer = undefined;
        this.program = undefined;
    }

    render = () => <canvas ref={this.canvasRef}></canvas>;
}
