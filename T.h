function calcularPoderTornado(
    objeto,          // `_0x35bc23`: Objeto com propriedades `a` e `b` que influenciam os cálculos
    valorReferencia, // `_0x41b0ef`: Referência de valor para o cálculo
    valorBase,       // `_0x2821ca`: Um valor base para ajustar as variáveis
    anguloMovimento, // `_0x201093`: O ângulo de movimento inicial
    distanciaMaxima, // `_0x58ca38`: A distância máxima a ser percorrida
    velocidade,      // `_0x563d20`: A velocidade do movimento
    anguloDirecao,   // `_0x605f00`: O ângulo de direção inicial
    coordenadaX,     // `_0x55bc4d`: Posição X de referência
    coordenadaY      // `_0x3c9416`: Posição Y de referência
) {
    let limiteInferior = -8;          // Limite inferior para as variáveis de controle
    let limiteSuperior = -60;         // Limite superior para o valor de referência
    let limiteMaximo = 2040;          // Limite máximo para o cálculo de posição
    let limiteDistancia = 5000;       // Limite máximo de distância permitida
    let distanciaCalculada;           // Armazena a distância calculada
    let distanciaMinima = 9999;       // Inicializa a distância mínima com um valor alto
    let deslocamentoX;                // Deslocamento em X
    let deslocamentoY;                // Deslocamento em Y
    let movimentoX;                   // Cálculo de movimento em X
    let movimentoY;                   // Cálculo de movimento em Y
    let anguloAtual;                  // Atualiza o ângulo de movimento
    let valorProximo;                 // Proximidade para atualização do movimento
    let distanciaFinal;               // Distância final calculada
    let iteracao = 0;                 // Contador para o loop

    // Converte o ângulo para radianos e calcula o deslocamento inicial
    const grauParaRadianos = Math.PI / 180;
    deslocamentoX = Math.round(Math.cos(anguloDirecao * grauParaRadianos) * velocidade) * objeto.b;
    deslocamentoY = Math.round(Math.sin(anguloDirecao * grauParaRadianos) * velocidade) * objeto.b - objeto.a;

    // Inicializa o movimento com valor zero
    movimentoX = 0;
    
    do {
        // Cálculo dos novos valores de posição baseados no ângulo
        movimentoX = Math.round(Math.cos(anguloMovimento * grauParaRadianos) * movimentoX);
        movimentoY = Math.round(Math.sin(anguloMovimento * grauParaRadianos) * movimentoX);

        // Atribui valores para as variáveis de referência e controle
        valorProximo = valorReferencia;
        distanciaFinal = 1200 - valorBase;
        distanciaCalculada = 1200 - coordenadaY;

        if (distanciaFinal <= 0) {
            movimentoX++; // Incrementa o contador
            continue;     // Continua o loop
        } else {
            while (1) {
                // Verificações para garantir que o movimento está dentro dos limites
                if (!limiteInferior || valorProximo <= limiteSuperior) {
                    break;
                }
                if (valorProximo >= limiteMaximo) {
                    break;
                }
                if (distanciaFinal >= limiteDistancia) {
                    break;
                }
                // Calcula o novo deslocamento em X e Y com base no valor de velocidade
                movimentoX += deslocamentoX * 0.0049;
                movimentoY += deslocamentoY * 0.0049;

                // Atualiza a posição e recalcula a distância
                valorProximo += movimentoX * 0.0049;
                distanciaFinal += movimentoY * 0.0049;

                // Calcula a distância entre a posição final e a posição de referência
                distanciaCalculada = Math.sqrt((coordenadaY - distanciaFinal) ** 2 + (coordenadaX - valorProximo) ** 2);

                // Se a nova distância for menor que a distância mínima, atualiza a distância mínima
                if (distanciaMinima > distanciaCalculada) {
                    distanciaMinima = distanciaCalculada;
                    iteracao = movimentoX;
                }
                if (distanciaFinal < 0) {
                    break;
                }
            }
        }
        movimentoX++; // Incrementa o contador de iteração
    } while (movimentoX <= 400); // Limita o número de iterações

    // Retorna o valor de "poder", que está relacionado à menor distância calculada
    return {
        poder: iteracao
    };
}






// Função para converter as posições do mundo para as coordenadas da câmera
function posToCamXY(positionX, positionY) {
    return {
        x: positionX - this.dragon2d.camera_x + 400,  // Calcula a posição X ajustada pela câmera
        y: positionY - this.dragon2d.camera_y + 300   // Calcula a posição Y ajustada pela câmera
    };
}

// Constantes para tipos de clima e tamanhos de efeitos
const WEATHER_MIRROR = 7;  // Tipo de clima "Espelho"
const WEATHER_TORNADO = 8; // Tipo de clima "Tornado"
const SIZE_TORNADO = 32;   // Tamanho do tornado
const SIZE_MIRROR = 25;    // Tamanho do espelho
const SIZE_ALL = 25;       // Tamanho padrão para outros tipos de clima

// Função para desenhar os efeitos climáticos na tela
function drawWeathers() {
    // Itera sobre todos os efeitos climáticos
    for (let weather of this.dragon2d.weathers) {
        const weatherType = weather.weather_type;  // Tipo do clima (espelho, tornado, etc.)
        let adjustedPositionY = weatherType === WEATHER_MIRROR ? -600 : -1500;  // Ajuste da posição Y para o tipo de clima
        let weatherPosition = posToCamXY.call(this, weather.position.x, adjustedPositionY); // Calcula a posição ajustada pela câmera
        // Ajusta o tamanho do clima dependendo do tipo e tamanho relativo
        let weatherSize = weatherType === WEATHER_MIRROR ? SIZE_MIRROR : weatherType === WEATHER_TORNADO ? SIZE_TORNADO * (weather.size / 128) : SIZE_ALL * (weather.size / 128);
        // Desenha as linhas representando o efeito climático
        this.ctx.moveTo(weatherPosition.x + weatherSize, weatherPosition.y);
        this.ctx.lineTo(weatherPosition.x + weatherSize, 1000);
        this.ctx.moveTo(weatherPosition.x - weatherSize, weatherPosition.y);
        this.ctx.lineTo(weatherPosition.x - weatherSize, 1000);
        this.ctx.stroke(); // Aplica o desenho
    }
    this.ctx.beginPath(); // Inicia um novo caminho de desenho
}

// Função para verificar as colisões dos efeitos climáticos com objetos ou eventos no tempo
function WeatherColision(timeOrObject) {
    // Itera sobre todos os efeitos climáticos
    for (let weather of this.dragon2d.weathers) {
        const currentWeatherType = weather.weather_type;  // Tipo atual de clima
        // Chama funções específicas para cada tipo de clima
        if (currentWeatherType === WEATHER_MIRROR) {
            WeatherMirror.call(this, weather, timeOrObject); // Verifica colisão para o clima "Espelho"
        }
        if (currentWeatherType === WEATHER_TORNADO) {
            WeatherTornado.call(this, weather, timeOrObject); // Verifica colisão para o clima "Tornado"
        }
    }
}

// Função para processar a colisão e efeitos do clima "Espelho"
function WeatherMirror(mirrorWeather, timeStamp) {
    // Obtém a posição atual no tempo para o objeto
    const currentPosition = this.zp.GetPosAtTime(timeStamp, jumps);
    let angle = this.zp.GetAngleAtTime(timeStamp, jumps); // Obtém o ângulo de movimento no tempo
    const distanceThreshold = 25; // Limite de distância para interação
    // Calcula a distância entre o clima "Espelho" e a posição atual
    const distanceToCurrentPosition = round(Math.sqrt(Math.pow(mirrorWeather.x - round(currentPosition.x), 2)));
    // Calcula a distância entre o clima "Espelho" e a posição Mex
    const distanceToMexPosition = round(Math.sqrt(Math.pow(mirrorWeather.x - round(this.mex), 2)));
    let isCloseEnough = distanceToMexPosition < distanceThreshold; // Verifica se está suficientemente perto
    // Verifica se o efeito climático deve ser disparado com base na distância e condições
    let shouldTriggerWeatherEffect = isCloseEnough ? mirrorWeather.x === Math.trunc(currentPosition.x) && round(currentPosition.y) > -600 && this.timeTemp < timeStamp : distanceToCurrentPosition < distanceThreshold && round(currentPosition.y) > -600 && this.timeTemp < timeStamp;
    if (shouldTriggerWeatherEffect) {
        // Se o efeito deve ser disparado, calcula a força do clima e aplica o efeito
        let weatherPower = this.zp.GetPowerAtTime(timeStamp, jumps);
        let weatherVector = new Vector(round(angle), round(weatherPower));  // Vetor de movimento baseado na força e ângulo
        let jumpData = [timeStamp, round(currentPosition.x), round(currentPosition.y), -round(weatherVector.x), -round(weatherVector.y)]; // Dados do salto
        jumps.push(jumpData); // Adiciona o salto aos saltos
        this.timeTemp = timeStamp + 10; // Define o tempo do efeito climático
    }
}

// Função para processar a colisão e efeitos do clima "Tornado"
function WeatherTornado(tornadoWeather, timeStamp) {
    // Ajusta o tamanho do tornado com base no tamanho relativo
    const adjustedTornadoSize = SIZE_TORNADO * (tornadoWeather.size / 128);
    // Obtém a posição do tornado no tempo atual
    let tornadoPosition = this.zp.GetPosAtTime(timeStamp, jumps);
    // Calcula a distância entre o tornado e a posição atual
    const distanceToTornado = round(Math.sqrt(Math.pow(tornadoWeather.x - round(tornadoPosition.x), 2)));
    // Verifica se o tornado está próximo o suficiente para iniciar o efeito
    if (distanceToTornado < adjustedTornadoSize && !JumpTornado) {
        let tornadoPositionAtTime = this.zp.GetPosAtTime(timeStamp, jumps);
        let tornadoAngleAtTime = -this.zp.GetAngleAtTime(timeStamp, jumps);
        let tornadoPower = this.zp.GetPowerAtTime(timeStamp, jumps);
        let tornadoVector = new Vector(round(tornadoAngleAtTime), round(tornadoPower)); // Vetor de movimento do tornado
        let tornadoJumpData = [timeStamp, round(tornadoPositionAtTime.x), round(tornadoPositionAtTime.y), round(tornadoVector.x), round(tornadoVector.y), 0, 0, 1]; // Dados do salto do tornado
        JumpTornado = 1; // Marca o início do efeito do tornado
        jumps.push(tornadoJumpData); // Adiciona o salto aos saltos
        jumpsT1 = tornadoJumpData; // Armazena o primeiro salto
    } else if (distanceToTornado > adjustedTornadoSize && distanceToTornado < adjustedTornadoSize + 2 && JumpTornado === 1) {
        const [jumpTime, jumpX, jumpY, jumpVectorX, jumpVectorY] = jumpsT1;
        let tornadoPositionAtTime = this.zp.GetPosAtTime(timeStamp, jumps);
        let tornadoJumpData2 = [timeStamp, tornadoPositionAtTime.x, tornadoPositionAtTime.y, -jumpVectorX, jumpVectorY, 0, 0]; // Dados do segundo salto
        JumpTornado = 2; // Atualiza o estado do tornado
        jumps.push(tornadoJumpData2); // Adiciona o salto aos saltos
        jumpsT2 = tornadoJumpData2; // Armazena o segundo salto
    } else if (distanceToTornado > adjustedTornadoSize && distanceToTornado < adjustedTornadoSize + 2 && JumpTornado === 2) {
        const [jumpTime, jumpX, jumpY, jumpVectorX, jumpVectorY] = jumpsT2;
        let tornadoPositionAtTime = this.zp.GetPosAtTime(timeStamp, jumps);
        let tornadoJumpData3 = [timeStamp, tornadoPositionAtTime.x, tornadoPositionAtTime.y, -jumpVectorX, jumpVectorY, 0, 0]; // Dados do terceiro salto
        JumpTornado = 3; // Atualiza o estado do tornado
        jumps.push(tornadoJumpData3); // Adiciona o salto aos saltos
        jumpsT3 = tornadoJumpData3; // Armazena o terceiro salto
    } else if (distanceToTornado > adjustedTornadoSize && distanceToTornado < adjustedTornadoSize + 2 && JumpTornado === 3) {
        const [jumpTime, jumpX, jumpY, jumpVectorX, jumpVectorY] = jumpsT3;
        let tornadoPositionAtTime = this.zp.GetPosAtTime(timeStamp, jumps);
        let finalTornadoJumpData = [timeStamp, tornadoPositionAtTime.x, tornadoPositionAtTime.y, jumpVectorX, jumpVectorY, round(this.zp.ax), round(this.zp.ay)]; // Dados finais do salto
        JumpTornado = 0; // Finaliza o efeito do tornado
        jumps.push(finalTornadoJumpData); // Adiciona o salto aos saltos
        jumpsT4 = finalTornadoJumpData; // Armazena o salto final
    }
}
