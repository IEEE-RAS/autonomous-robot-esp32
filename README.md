# 🤖 ROBOESP - Diário de Desenvolvimento

**Equipe:** IEEE RAS - UEFS

## 📝 Visão Geral

Este repositório serve como o diário de desenvolvimento oficial do projeto ROBOESP. Nosso objetivo é desenvolver um robô explorador autônomo, o ROBOESP, capaz de coletar dados ambientais através de múltiplos sensores. O robô utiliza um modelo de inteligência artificial embarcado para analisar os dados e prever a "probabilidade de vida" em tempo real. As informações são transmitidas a um servidor central (Broker) que as disponibiliza para um dashboard web, permitindo o monitoramento e controle remoto do sistema.

Atualmente, o projeto está numa fase de reavaliação e expansão. A prova de conceito inicial utilizou potenciômetros para simular as leituras dos sensores. Agora, o foco é na seleção e integração de sensores reais, na adição de novas funcionalidades como visão computacional, e na otimização do nosso modelo de IA.

## 👥 Equipe

| Nome                 | Cargo no projeto             | GitHub                                     |
| -------------------- | ------------------------- | ------------------------------------------ |
| Silvio Azevedo    | Líder de Projeto | [@Silviozv](https://github.com/Silviozv)   |
| Yasmin Cordeiro  | Secretario de Mesa   | [@yasmincsme](https://github.com/yasmincsme)   |
| Naila Suele    | Projetista   | [@nailasuely](https://github.com/nailasuely)   |

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

| Categoria      | Ferramentas                                                        |
| -------------- | ------------------------------------------------------------------ |
| **Linguagens** | C/C++ (ESP32), Python (Broker), HTML, CSS, JavaScript (Dashboard) |
| **Frameworks** | Flask, Flask-CORS (Python), Chart.js (JS), ArduinoJson, UniversalTelegramBot (C++) |
| **Hardware** | ESP32, Sensores (Temperatura, Umidade, Gás, Luminosidade), Webcam  |
| **Banco de Dados** | SQLite                                                           |


-----

## 📓 Diário de Desenvolvimento

Aqui registramos nosso progresso semanalmente. Cada entrada inclui atualizações de hardware e software, os desafios que encontramos e as decisões que tomamos.

### Semana de 22/09/2025 - 28/09/2025

**Foco da Semana:** Reavaliação Estratégica e Planejamento

* **Decisões de Projeto (Reunião de 27/09):**
    * **Rede Neural:** O grupo está a considerar refazer a rede neural. O plano inclui a seleção de novos sensores reais (para substituir os potenciômetros de simulação), a recolha de um novo dataset com base nos ranges desses sensores, e o retreinamento do modelo para maior precisão.
    * **Publicação:** Decidimos não focar em congressos da área de educação para manter abertas mais opções de publicação em locais de maior impacto técnico.

* **Tarefas da Semana:**
    * **Naila:** Responsável por atualizar o repositório no GitHub, subindo os arquivos da aplicação web (dashboard e broker) e revisando o README.
    * **Yasmin:** Encarregada de pesquisar módulos de câmara, focando em custo e especificações técnicas para a futura integração.
    * **Toda a equipe:** Realizar uma pesquisa de trabalhos académicos e possíveis locais para publicação, cobrindo os escopos de IoT, IA, sensoriamento, agricultura inteligente e visão computacional (mínimo de 1 trabalho por escopo).

* **Gestão de Projeto:**
    * As reuniões semanais foram fixadas para o horário das 13h às 13h30 e agendadas no Google Agenda da equipa.


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
