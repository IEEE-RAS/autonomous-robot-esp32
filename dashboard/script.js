document.addEventListener('DOMContentLoaded', () => {

    // --- CONFIGURAÇÕES GLOBAIS ---
    const BROKER_IP = "localhost";
    const BROKER_PORT = 5001;
    const DEVICE_NAME = "RoboExplorador";
    const API_URL = `http://${BROKER_IP}:${BROKER_PORT}/devices/${DEVICE_NAME}`;
    const LATEST_PHOTO_INFO_URL = `http://${BROKER_IP}:${BROKER_PORT}/photos/latest`;

    // Intervalos de atualização
    const UPDATE_INTERVAL = 2000;
    const PHOTO_UPDATE_INTERVAL = 5000;

    // --- ELEMENTOS DO DOM ---
    const statusDot = document.getElementById('status-dot');
    const statusText = document.getElementById('status-text');
    const lifeProbValue = document.getElementById('life-prob-value');
    const classificationText = document.getElementById('classification-text');
    const tempValue = document.getElementById('temp-value');
    const humidityValue = document.getElementById('humidity-value');
    const gasValue = document.getElementById('gas-value');
    const lightValue = document.getElementById('light-value');
    const stopButton = document.getElementById('stop-button');
    const gaugeFill = document.querySelector('.gauge-body');
    const robotPhoto = document.getElementById('robot-photo');
    const photoStatusText = document.getElementById('photo-status');
    const themeToggleButton = document.getElementById('theme-toggle-button');
    const themeIcon = document.getElementById('theme-icon');

    // --- VARIÁVEIS DE ESTADO ---
    let tempHumidityChart, gasLightChart;

    // --- LÓGICA DE TEMA ---
    const applyTheme = (theme) => {
        document.body.className = theme;
        themeIcon.className = theme === 'light-mode' ? 'fa-regular fa-sun' : 'fa-regular fa-moon';
        if (tempHumidityChart) tempHumidityChart.destroy();
        if (gasLightChart) gasLightChart.destroy();
        initializeCharts();
    };

    themeToggleButton.addEventListener('click', () => {
        const newTheme = document.body.classList.contains('light-mode') ? 'dark-mode' : 'light-mode';
        applyTheme(newTheme);
    });

    // --- LÓGICA DE GRÁFICOS ---
    const getChartColors = () => {
        const styles = getComputedStyle(document.documentElement);
        return {
            accent: styles.getPropertyValue('--accent-color').trim(),
            accentLight: styles.getPropertyValue('--accent-light').trim(),
            borderColor: styles.getPropertyValue('--border-color').trim(),
            textColor: styles.getPropertyValue('--text-color').trim(),
            textSecondary: styles.getPropertyValue('--text-secondary').trim()
        };
    };

    const createChart = (canvasId, datasetsConfig) => {
        const colors = getChartColors();
        const ctx = document.getElementById(canvasId).getContext('2d');
        return new Chart(ctx, {
            type: 'line',
            data: { labels: [], datasets: datasetsConfig },
            options: {
                responsive: true, maintainAspectRatio: false,
                scales: {
                    x: { ticks: { color: colors.textSecondary }, grid: { color: colors.borderColor } },
                    y: { ticks: { color: colors.textSecondary }, grid: { color: colors.borderColor } }
                },
                plugins: { legend: { labels: { color: colors.textColor, font: { family: "'DM Sans', sans-serif" } } } },
                interaction: { intersect: false, mode: 'index' }
            }
        });
    };

    const initializeCharts = () => {
        const colors = getChartColors();
        const commonOptions = { borderWidth: 2, fill: true, tension: 0.4, pointRadius: 0, pointHoverRadius: 5 };
        tempHumidityChart = createChart('tempHumidityChart', [
            { label: 'Temperatura (°C)', data: [], borderColor: colors.accent, backgroundColor: `${colors.accent}33`, ...commonOptions },
            { label: 'Umidade (%)', data: [], borderColor: colors.accentLight, backgroundColor: `${colors.accentLight}33`, ...commonOptions }
        ]);
        gasLightChart = createChart('gasLightChart', [
            { label: 'Gás (ppm)', data: [], borderColor: '#FFB547', backgroundColor: '#FFB54733', ...commonOptions },
            { label: 'Luz (cd)', data: [], borderColor: '#39A2DB', backgroundColor: '#39A2DB33', ...commonOptions }
        ]);
    };

    const updateChartData = (chart, newLabel, newValues) => {
        if (!chart) return;
        chart.data.labels.push(newLabel);
        chart.data.datasets.forEach((dataset, index) => dataset.data.push(newValues[index]));
        if (chart.data.labels.length > 30) {
            chart.data.labels.shift();
            chart.data.datasets.forEach(dataset => dataset.data.shift());
        }
        chart.update();
    };

    // --- LÓGICA DE DADOS (ATUALIZADA COM MELHOR FEEDBACK) ---
    const updateDashboard = async () => {
        try {
            const response = await fetch(`${API_URL}/data`);
            const responseData = await response.json();

            if (!response.ok) {
                throw new Error(responseData.message || `Erro de rede: ${response.status}`);
            }

            if (responseData.status === 'waiting_for_data') {
                statusDot.className = 'dot connected';
                statusText.textContent = responseData.message || 'Aguardando dados...';
                return;
            }
            if (!responseData.data) {
                throw new Error('Formato de dados da API inválido.');
            }
            
            statusDot.className = 'dot connected';
            statusText.textContent = 'Online e recebendo dados';
            
            const { life_chance, temp, hum, gas, lux, terrain_status } = responseData.data;
            updateUI(life_chance, temp, hum, gas, lux, terrain_status);

        } catch (error) {
            console.error("Erro ao atualizar dashboard:", error);
            statusDot.className = 'dot disconnected';
            statusText.textContent = error.message; // Exibe a mensagem de erro específica
        }
    };
    
    const updateUI = (lifeProb, temp, humidity, gas, light, classification) => {
        const colors = getChartColors();
        lifeProbValue.textContent = `${Math.round(lifeProb * 100)}%`;
        classificationText.textContent = `Classificação: ${classification}`;
        tempValue.textContent = parseFloat(temp).toFixed(1);
        humidityValue.textContent = parseFloat(humidity).toFixed(1);
        gasValue.textContent = parseFloat(gas).toFixed(1);
        lightValue.textContent = parseFloat(light).toFixed(1);
        
        const angle = lifeProb * 360;
        gaugeFill.style.background = `conic-gradient(${colors.accent} ${angle}deg, ${colors.borderColor} ${angle}deg)`;

        const currentTime = new Date().toLocaleTimeString('pt-BR');
        updateChartData(tempHumidityChart, currentTime, [temp, humidity]);
        updateChartData(gasLightChart, currentTime, [gas, light]);
    };

    // --- FUNÇÃO DE FOTO (ATUALIZADA COM MELHOR FEEDBACK) ---
    const updatePhoto = async () => {
        try {
            const response = await fetch(LATEST_PHOTO_INFO_URL);
            if (!response.ok) {
                photoStatusText.textContent = `Erro (${response.status}) ao buscar foto.`;
                robotPhoto.style.display = 'none';
                return;
            }
            const data = await response.json();
            if (data && data.photo_url) {
                const fullPhotoUrl = `http://${BROKER_IP}:${BROKER_PORT}${data.photo_url}?t=${new Date().getTime()}`;
                robotPhoto.src = fullPhotoUrl;
                robotPhoto.style.display = 'block';
                photoStatusText.style.display = 'none';
            } else {
                photoStatusText.textContent = "Nenhuma foto disponível.";
                photoStatusText.style.display = 'block';
                robotPhoto.style.display = 'none';
            }
        } catch (error) {
            console.error("Erro crítico na função updatePhoto:", error);
            photoStatusText.textContent = "Erro de conexão ao buscar foto.";
            photoStatusText.style.display = 'block';
            robotPhoto.style.display = 'none';
        }
    };

     stopButton.addEventListener('click', async () => {
        try {
            const response = await fetch(`${API_URL}/command`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ command_type: 'toggle_system_state' })
            });
            const result = await response.json();
            if(response.ok) {
                // Em um ambiente real, você usaria uma modalbox personalizada em vez de alert()
                alert('Comando enviado com sucesso: ' + result.message); 
            } else {
                alert('Falha ao enviar comando: ' + result.error);
            }
        } catch (error) {
            alert('Erro de conexão ao enviar comando.');
            console.error("Erro ao enviar comando:", error);
        }
    });
    
    // --- INICIALIZAÇÃO ---
    applyTheme(document.body.className || 'dark-mode');
    updateDashboard();
    updatePhoto();
    setInterval(updateDashboard, UPDATE_INTERVAL);
    setInterval(updatePhoto, PHOTO_UPDATE_INTERVAL);
});