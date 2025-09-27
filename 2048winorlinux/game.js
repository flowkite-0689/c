// 初始化3D场景
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

// 创建调试信息元素
const debugInfo = document.createElement('div');
debugInfo.style.position = 'absolute';
debugInfo.style.top = '60px';
debugInfo.style.left = '20px';
debugInfo.style.color = 'white';
debugInfo.style.backgroundColor = 'rgba(0,0,0,0.5)';
debugInfo.style.padding = '5px';
debugInfo.style.borderRadius = '3px';
document.body.appendChild(debugInfo);

try {
    // 检查WebGL支持
    if (!window.WebGLRenderingContext) {
        throw new Error('浏览器不支持WebGL');
    }

    // 尝试获取WebGL上下文
    const canvas = document.createElement('canvas');
    const gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl');
    if (!gl) {
        throw new Error('无法获取WebGL上下文');
    }

    debugInfo.textContent += `\nWebGL供应商: ${gl.getParameter(gl.VENDOR)}`;
    debugInfo.textContent += `\nWebGL渲染器: ${gl.getParameter(gl.RENDERER)}`;

    // 创建渲染器
    const renderer = new THREE.WebGLRenderer({
        antialias: true,
        alpha: true,
        powerPreference: 'high-performance'
    });

    if (!renderer) {
        throw new Error('无法创建WebGL渲染器');
    }

    // 设置渲染器错误回调
    renderer.debug.checkShaderErrors = true;

    // 初始化rendererCanvas
    const rendererCanvas = renderer.domElement;

    // 添加WebGL上下文丢失处理
    rendererCanvas.addEventListener('webglcontextlost', (event) => {
        debugInfo.innerHTML = '<h3 style="color:red">WebGL上下文丢失!</h3>' +
                             '<p>可能由于系统资源不足或浏览器限制</p>';
        console.error('WebGL上下文丢失:', event);
    }, false);

    renderer.setSize(window.innerWidth, window.innerHeight);

    // 强制触发一次渲染检查
    renderer.render(scene, camera);
    document.body.appendChild(renderer.domElement);

    // 调试信息 - 检查canvas元素
    debugInfo.textContent += `\nCanvas尺寸: ${rendererCanvas.width}x${rendererCanvas.height}`;

    // 详细检查WebGL上下文状态
    gl = rendererCanvas.getContext('webgl') || rendererCanvas.getContext('experimental-webgl');
    if (!gl) {
        const errorInfo = [];
        // 检查常见阻止原因
        if (/iPad|iPhone|iPod/.test(navigator.userAgent)) {
            errorInfo.push('iOS设备可能需要手动启用WebGL');
        }
        if (navigator.userAgent.includes('Chrome') && !navigator.userAgent.includes('Edge')) {
            errorInfo.push('Chrome浏览器: 检查chrome://flags/#enable-webgl');
        }
        if (navigator.userAgent.includes('Safari') && !navigator.userAgent.includes('Chrome')) {
            errorInfo.push('Safari浏览器: 需在"开发"菜单中启用WebGL');
        }

        debugInfo.textContent += `\nWebGL上下文: 未获取 - 可能原因: ${errorInfo.join('; ') || '未知'}`;
        throw new Error(`WebGL上下文获取失败: ${errorInfo.join('; ') || '未知原因'}`);
    } else {
        debugInfo.textContent += `\nWebGL上下文: 已获取 (版本: ${gl.getParameter(gl.VERSION)})`;
    }

    // 添加光源
    const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
    scene.add(ambientLight);
    const directionalLight = new THREE.DirectionalLight(0xffffff, 0.5);
    directionalLight.position.set(1, 1, 1);
    scene.add(directionalLight);

    // 设置场景背景色
    scene.background = new THREE.Color(0x333333);

    // 调试相机位置
    camera.position.set(0, 0, 8);
    camera.lookAt(0, 0, 0);
    debugInfo.textContent += `\n相机位置: (${camera.position.x}, ${camera.position.y}, ${camera.position.z})`;

    // 添加辅助坐标系
    const axesHelper = new THREE.AxesHelper(5);
    scene.add(axesHelper);

    debugInfo.textContent += '\n场景初始化完成';
} catch (error) {
    debugInfo.innerHTML = `
        <h3 style="color:red;margin:0">渲染错误: ${error.message}</h3>
        <p>您的浏览器或硬件可能不支持WebGL</p>
        <p>建议解决方案:</p>
        <ol>
            <li>更新浏览器到最新版本</li>
            <li>检查浏览器设置中WebGL是否启用</li>
            <li>尝试使用Chrome或Firefox浏览器</li>
            <li>检查显卡驱动是否最新</li>
        </ol>
        <p>错误详情: ${error.stack}</p>
    `;
    debugInfo.style.color = 'red';
    debugInfo.style.width = '80%';
    debugInfo.style.maxWidth = '600px';
    debugInfo.style.padding = '20px';
    console.error('WebGL初始化错误:', error);

    // 创建备用canvas用于错误显示
    const errorCanvas = document.createElement('canvas');
    errorCanvas.width = 400;
    errorCanvas.height = 200;
    errorCanvas.style.border = '1px solid red';
    errorCanvas.style.margin = '20px auto';
    errorCanvas.style.display = 'block';

    const ctx = errorCanvas.getContext('2d');
    ctx.fillStyle = 'white';
    ctx.fillRect(0, 0, errorCanvas.width, errorCanvas.height);
    ctx.fillStyle = 'red';
    ctx.font = '16px Arial';
    ctx.fillText('WebGL不支持 - 无法渲染3D场景', 20, 50);
    ctx.fillText(error.message, 20, 80);

    document.body.appendChild(errorCanvas);
}

// 创建3D网格
const gridSize = 4;
const cubes = [];

// 初始化游戏
function initGame() {
    debugInfo.textContent += '\n开始初始化游戏网格...';

    // 创建3D网格
    for (let x = 0; x < gridSize; x++) {
        cubes[x] = [];
        for (let y = 0; y < gridSize; y++) {
            cubes[x][y] = [];
            for (let z = 0; z < gridSize; z++) {
                // 创建方块
                const geometry = new THREE.BoxGeometry(0.9, 0.9, 0.9);
                const material = new THREE.MeshBasicMaterial({
                    color: 0x00ff00,
                    transparent: true,
                    opacity: 0.8  // 提高不透明度以便观察
                });
                const cube = new THREE.Mesh(geometry, material);
                const posX = x - gridSize/2;
                const posY = y - gridSize/2;
                const posZ = z - gridSize/2;
                cube.position.set(posX, posY, posZ);
                scene.add(cube);
                cubes[x][y][z] = { mesh: cube, value: 0 };

                // 调试信息 - 第一个方块的位置
                if (x === 0 && y === 0 && z === 0) {
                    debugInfo.textContent += `\n第一个方块位置: (${posX}, ${posY}, ${posZ})`;
                }
            }
        }
    }

    debugInfo.textContent += '\n游戏网格初始化完成';

    // 添加随机方块
    addRandomCube();
    addRandomCube();
}

// 添加随机方块
function addRandomCube() {
    const emptyCells = [];
    for (let x = 0; x < gridSize; x++) {
        for (let y = 0; y < gridSize; y++) {
            for (let z = 0; z < gridSize; z++) {
                if (cubes[x][y][z].value === 0) {
                    emptyCells.push({x, y, z});
                }
            }
        }
    }

    if (emptyCells.length > 0) {
        const cell = emptyCells[Math.floor(Math.random() * emptyCells.length)];
        cubes[cell.x][cell.y][cell.z].value = Math.random() < 0.9 ? 2 : 4;
        updateCubeAppearance(cell.x, cell.y, cell.z);
    }
}

// 更新方块外观
function updateCubeAppearance(x, y, z) {
    const cube = cubes[x][y][z];
    const value = cube.value;

    // 根据数值设置不同颜色
    const colors = {
        0: 0x00ff00, 2: 0xeee4da, 4: 0xede0c8,
        8: 0xf2b179, 16: 0xf59563, 32: 0xf67c5f,
        64: 0xf65e3b, 128: 0xedcf72, 256: 0xedcc61,
        512: 0xedc850, 1024: 0xedc53f, 2048: 0xedc22e
    };

    cube.mesh.material.color.setHex(colors[value] || 0x3c3a32);
    cube.mesh.material.opacity = value === 0 ? 0.3 : 1;
}

// 动画循环
function animate() {
    try {
        requestAnimationFrame(animate);
        if (renderer && scene && camera) {
            renderer.render(scene, camera);

            // 调试信息
            if (typeof debugInfo !== 'undefined') {
                debugInfo.textContent = `渲染中... 相机位置: (${camera.position.x.toFixed(1)},
                                    ${camera.position.y.toFixed(1)},
                                    ${camera.position.z.toFixed(1)})`;
            }
        }
    } catch (error) {
        if (typeof debugInfo !== 'undefined') {
            debugInfo.textContent = `渲染错误: ${error.message}`;
            debugInfo.style.color = 'red';
        }
        console.error('渲染错误:', error);
    }
}

// 设置相机位置
camera.position.z = 8;

// 键盘控制
document.addEventListener('keydown', (event) => {
    let moved = false;

    switch(event.key) {
        case 'ArrowUp':
            moved = moveCubes(0, 1, 0);
            break;
        case 'ArrowDown':
            moved = moveCubes(0, -1, 0);
            break;
        case 'ArrowLeft':
            moved = moveCubes(-1, 0, 0);
            break;
        case 'ArrowRight':
            moved = moveCubes(1, 0, 0);
            break;
        case 'w':
            moved = moveCubes(0, 0, 1);
            break;
        case 's':
            moved = moveCubes(0, 0, -1);
            break;
    }

    if (moved) {
        addRandomCube();
    }
});

// 移动方块
function moveCubes(dx, dy, dz) {
    let moved = false;
    let score = 0;

    // 根据移动方向确定遍历顺序
    const xStart = dx > 0 ? gridSize-1 : 0;
    const xEnd = dx > 0 ? -1 : gridSize;
    const xStep = dx > 0 ? -1 : 1;

    const yStart = dy > 0 ? gridSize-1 : 0;
    const yEnd = dy > 0 ? -1 : gridSize;
    const yStep = dy > 0 ? -1 : 1;

    const zStart = dz > 0 ? gridSize-1 : 0;
    const zEnd = dz > 0 ? -1 : gridSize;
    const zStep = dz > 0 ? -1 : 1;

    // 遍历所有方块
    for (let x = xStart; x !== xEnd; x += xStep) {
        for (let y = yStart; y !== yEnd; y += yStep) {
            for (let z = zStart; z !== zEnd; z += zStep) {
                if (cubes[x][y][z].value === 0) continue;

                let newX = x, newY = y, newZ = z;
                let canMove = true;

                // 尝试移动方块
                while (canMove) {
                    const nextX = newX + dx;
                    const nextY = newY + dy;
                    const nextZ = newZ + dz;

                    // 检查边界
                    if (nextX < 0 || nextX >= gridSize ||
                        nextY < 0 || nextY >= gridSize ||
                        nextZ < 0 || nextZ >= gridSize) {
                        canMove = false;
                        break;
                    }

                    const nextCube = cubes[nextX][nextY][nextZ];

                    // 遇到空方块则移动
                    if (nextCube.value === 0) {
                        newX = nextX;
                        newY = nextY;
                        newZ = nextZ;
                        moved = true;
                    }
                    // 遇到相同数值则合并
                    else if (nextCube.value === cubes[x][y][z].value) {
                        nextCube.value *= 2;
                        score += nextCube.value;
                        cubes[x][y][z].value = 0;
                        updateCubeAppearance(nextX, nextY, nextZ);
                        updateCubeAppearance(x, y, z);
                        moved = true;
                        canMove = false;
                    } else {
                        canMove = false;
                    }
                }

                // 执行移动
                if (newX !== x || newY !== y || newZ !== z) {
                    cubes[newX][newY][newZ].value = cubes[x][y][z].value;
                    cubes[x][y][z].value = 0;
                    updateCubeAppearance(newX, newY, newZ);
                    updateCubeAppearance(x, y, z);
                }
            }
        }
    }

    if (score > 0) {
        document.getElementById('score').textContent = parseInt(document.getElementById('score').textContent) + score;
    }

    return moved;
}

// 检查游戏是否结束
function checkGameOver() {
    for (let x = 0; x < gridSize; x++) {
        for (let y = 0; y < gridSize; y++) {
            for (let z = 0; z < gridSize; z++) {
                if (cubes[x][y][z].value === 0) return false;

                // 检查相邻方块是否可以合并
                const directions = [
                    [1,0,0], [-1,0,0], [0,1,0],
                    [0,-1,0], [0,0,1], [0,0,-1]
                ];

                for (const [dx, dy, dz] of directions) {
                    const nx = x + dx;
                    const ny = y + dy;
                    const nz = z + dz;

                    if (nx >= 0 && nx < gridSize &&
                        ny >= 0 && ny < gridSize &&
                        nz >= 0 && nz < gridSize &&
                        cubes[nx][ny][nz].value === cubes[x][y][z].value) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

// 重置游戏
function resetGame() {
    scene.children = scene.children.filter(child => child.type !== 'Mesh');
    document.getElementById('score').textContent = '0';
    document.getElementById('game-over').style.display = 'none';
    initGame();
}

// 启动游戏
resetGame();
