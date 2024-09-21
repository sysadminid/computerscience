let scene, camera, renderer, neuralCloud;
let isDragging = false;
let previousMousePosition = { x: 0, y: 0 };
const particleCount = 170;
const maxConnections = 30;
let cloudSize = 20;

function init() {
    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera(40, window.innerWidth / window.innerHeight, 0.1, 1000);
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.getElementById('neural-particles').appendChild(renderer.domElement);

    updateCloudSize();
    neuralCloud = createNeuralCloud();
    scene.add(neuralCloud);

    // Initial camera position
    camera.position.z = cloudSize * 3;

    // Call resize and size update functions
    updateCloudSize();
    onWindowResize(); // Ensure proper sizing after initial load

    animate();
}

function updateCloudSize() {
    const container = document.getElementById('neural-particles');
    const maxDimension = Math.max(container.clientWidth, container.clientHeight);
    cloudSize = maxDimension / 300;

    // Update neural cloud and camera position based on new cloudSize
    if (neuralCloud) {
        neuralCloud.geometry.dispose();
        scene.remove(neuralCloud);
        neuralCloud = createNeuralCloud();
        scene.add(neuralCloud);
    }

    if (camera) {
        camera.position.z = cloudSize * 3;
    }
}

function createNeuralCloud() {
    const geometry = new THREE.BufferGeometry();
    const positions = new Float32Array(particleCount * 3);
    const connections = [];

    for (let i = 0; i < particleCount; i++) {
        const theta = Math.random() * Math.PI * 2;
        const phi = Math.acos(2 * Math.random() - 1);
        const r = cloudSize * Math.cbrt(Math.random());

        positions[i * 3] = r * Math.sin(phi) * Math.cos(theta);
        positions[i * 3 + 1] = r * Math.sin(phi) * Math.sin(theta);
        positions[i * 3 + 2] = r * Math.cos(phi);
    }

    geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));

    for (let i = 0; i < particleCount; i++) {
        const nodeConnections = [];
        for (let j = 0; j < particleCount; j++) {
            if (i !== j) {
                const distance = Math.sqrt(
                    Math.pow(positions[i * 3] - positions[j * 3], 2) +
                    Math.pow(positions[i * 3 + 1] - positions[j * 3 + 1], 2) +
                    Math.pow(positions[i * 3 + 2] - positions[j * 3 + 2], 2)
                );
                nodeConnections.push({ index: j, distance: distance });
            }
        }
        nodeConnections.sort((a, b) => a.distance - b.distance);
        const connectCount = Math.floor(Math.random() * maxConnections) + 1;
        for (let k = 0; k < connectCount && k < nodeConnections.length; k++) {
            connections.push(i, nodeConnections[k].index);
        }
    }

    geometry.setIndex(connections);
    const material = new THREE.LineBasicMaterial({ color: 0xffffff, transparent: true, opacity: 0.21 });
    return new THREE.LineSegments(geometry, material);
}

function animate() {
    requestAnimationFrame(animate);

    neuralCloud.rotation.y +=  0.002;
    neuralCloud.rotation.x += -0.002;

    renderer.render(scene, camera);
}

function onMouseDown(event) {
    isDragging = true;
    previousMousePosition.x = event.clientX;
    previousMousePosition.y = event.clientY;
    document.getElementById('neural-particles').style.cursor = 'grabbing';
}

function onMouseMove(event) {
    if (isDragging) {
        const deltaMove = {
            x: event.clientX - previousMousePosition.x,
            y: event.clientY - previousMousePosition.y
        };

        const rotationSpeed = 0.005;
        neuralCloud.rotation.y += deltaMove.x * rotationSpeed;
        neuralCloud.rotation.x += deltaMove.y * rotationSpeed;

        previousMousePosition = {
            x: event.clientX,
            y: event.clientY
        };
    }
}

function onMouseUp() {
    isDragging = false;
    document.getElementById('neural-particles').style.cursor = 'grab';
}

function onWindowResize() {
    const container = document.getElementById('neural-particles');
    const width = container.clientWidth;
    const height = container.clientHeight - 270;

    renderer.setSize(width, height);
    camera.aspect = width / height;
    camera.updateProjectionMatrix();
    
    // Update cloud size and position on resize
    updateCloudSize();
}

window.addEventListener('resize', onWindowResize, false);
document.addEventListener('mousedown', onMouseDown, false);
document.addEventListener('mousemove', onMouseMove, false);
document.addEventListener('mouseup', onMouseUp, false);

// Ensure proper setup after the page loads
window.addEventListener('load', init, false);
