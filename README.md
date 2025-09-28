# 🤖 [Modelo de Diario de desenvolvimento] - Diário de Desenvolvimento

**Equipe:** IEEE RAS - UEFS

## 📝 Visão Geral

Este repositório serve como o diário de desenvolvimento oficial do projeto [Nome do Projeto]. Nosso objetivo é [descrever o objetivo principal do projeto em uma ou duas frases, por exemplo: "construir um robô autônomo para competições de robótica" ou "desenvolver um sistema de visão computacional para manipulação de objetos"]. Aqui, documentamos nosso progresso, desafios e aprendizados em todas as frentes: software, hardware, eletrônica e design mecânico.

## 👥 Equipe

| Nome                 | Cargo no projeto             | GitHub                                     |
| -------------------- | ------------------------- | ------------------------------------------ |
| [Nome do Membro 1]   | Líder de Projeto | [@username](https://www.google.com/search?q=https://github.com/username)   |
| [Nome do Membro 2]   | Secretario de Mesa   | [@username](https://www.google.com/search?q=https://github.com/username)   |
| [Nome do Membro 3]   | Projetista   | [@username](https://www.google.com/search?q=https://github.com/username)   |
| [Nome do Membro 4]   | Projetista             | [@username](https://www.google.com/search?q=https://github.com/username)   |
| *Adicionar mais membros* |                           |                                            |

## 🎯 Metas do Projeto

### Metas atuais

  * **Hardware:** [Ex: Finalizar o design da PCB para os drivers de motor].
  * **Software:** [Ex: Implementar o algoritmo de desvio de obstáculos].
  * **Modelagem 3D:** [Ex: Imprimir a versão 2.1 do chassi e validar as montagens].

### Objetivos Gerais

  - **Hardware:**
      - [ ] Projetar e montar um chassi robusto e modular.
      - [ ] Desenvolver uma placa de circuito impresso (PCB) customizada para gerenciamento de energia e sensores.
      - [ ] Integrar [listar sensores e atuadores, ex: Lidar, câmera, motores DC].
  - **Software:**
      - [ ] Desenvolver um sistema de controle de baixo nível para os atuadores.
      - [ ] Implementar algoritmos de localização e mapeamento simultâneo (SLAM).
      - [ ] Criar uma interface de usuário para monitoramento e controle remoto.

## 🛠️ Tecnologias e Ferramentas

| Categoria         | Ferramentas                                       |
| ----------------- | ------------------------------------------------- |
| **Linguagens** | C++, Python                                       |
| **Frameworks** | ROS (Robot Operating System), OpenCV, TensorFlow  |
| **Hardware** | Raspberry Pi 4, Arduino Nano, ESP32               |
| **Modelagem 3D** | SolidWorks, Fusion 360, Onshape                   |
| **Design de PCB** | KiCad, Altium Designer, Eagle                     |
| **Simulação** | Gazebo, CoppeliaSim (V-REP)                       |

-----

## 📓 Diário de Desenvolvimento

Aqui registramos nosso progresso semanalmente. Cada entrada inclui atualizações de hardware e software, os desafios que encontramos e as decisões que tomamos.

### Semana 1: (DD/MM/AAAA) - (DD/MM/AAAA)

**Foco da Semana:** Planejamento Inicial e Prototipagem

  * **Software:**
      * **Arquitetura:** Definimos a arquitetura de software baseada em nós do ROS. A comunicação entre os nós de percepção, controle e planejamento foi esquematizada.
      * **Código:** Criamos a estrutura inicial do repositório e os pacotes ROS essenciais. `[Link para o Commit ou Pull Request]`
  * **Hardware & Modelagem 3D:**
      * **Modelagem:** O primeiro rascunho do chassi foi modelado no SolidWorks, focando em um design modular para fácil manutenção.
          * *Arquivo CAD:* `[Link para a pasta /cad/v1.0]`
      * **Eletrônica:** Selecionamos os componentes eletrônicos principais e desenhamos o esquemático preliminar no KiCad.
  * **Desafios:**
      * Decidir qual microcontrolador principal usar (Raspberry Pi vs. Jetson Nano) foi um grande debate. A escolha impacta o custo e a capacidade de processamento de IA.
  * **Decisões:**
      * Optamos pela Raspberry Pi 4 para a fase inicial devido à familiaridade da equipe e ao vasto suporte da comunidade.

### Semana 2: (DD/MM/AAAA) - (DD/MM/AAAA)

**Foco da Semana:** Impressão do Chassi e Integração de Sensores

  * **Software:**
      * **Código:** Implementamos o driver para o sensor de unidade de medição inercial (IMU) e publicamos os dados em um tópico ROS. `[Link para o Commit]`
      * **Testes:** Realizamos testes iniciais para visualizar os dados da IMU no Rviz.
  * **Hardware & Modelagem 3D:**
      * **Impressão 3D:** A primeira versão do chassi foi impressa em PLA. Identificamos alguns pontos de melhoria na rigidez estrutural.
      * **Montagem:** Iniciamos a montagem dos motores e da IMU no chassi.
  * **Desafios:**
      * O ruído nos dados da IMU foi maior do que o esperado, exigindo a pesquisa e implementação de filtros de software (ex: Filtro de Kalman).
  * **Próximos Passos:**
      * Reforçar o design do chassi na modelagem 3D.
      * Implementar um filtro de software para os dados da IMU.

**(Continue este formato para as semanas seguintes)**

## 🚀 Como Contribuir

Somos um projeto de código e hardware abertos\! Se você deseja contribuir:

1. **Faça um Fork** deste repositório.
2. **Crie uma Branch** para sua feature (`git checkout -b feature/SuaFeature`).
3. **Faça o Commit** de suas mudanças (`git commit -m 'Adiciona SuaFeature'`).
4. **Faça o Push** para a Branch (`git push origin feature/SuaFeature`).
5. Abra um **Pull Request**.

Para problemas ou sugestões, por favor, abra uma **Issue**.

## 📄 Licença

Este projeto está licenciado sob a Licença GNU V2.0 (ou a licensa que estiverem utilizando, se estiverem). Veja o arquivo [LICENSE.md](LICENSE.md) para mais detalhes.