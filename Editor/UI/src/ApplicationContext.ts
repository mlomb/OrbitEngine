declare var __ENV__: any;

const devMode = __ENV__.NODE_ENV !== 'production';

export const isDevMode = devMode;

console.log('isDevMode', isDevMode);
