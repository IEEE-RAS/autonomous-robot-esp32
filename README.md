# 🤖 ROBOESP - Diário de Desenvolvimento

**Equipe:** IEEE RAS - UEFS

## 📝 Visão Geral

Este repositório serve como o diário de desenvolvimento oficial do projeto ROBOESP. Nosso objetivo é desenvolver um robô explorador autônomo, o ROBOESP, capaz de coletar dados ambientais através de múltiplos sensores. O robô utiliza um modelo de inteligência artificial embarcado para analisar os dados e prever a "probabilidade de vida" em tempo real. As informações são transmitidas a um servidor central (Broker) que as disponibiliza para um dashboard web, permitindo o monitoramento e controle remoto do sistema.

Atualmente, o projeto está numa fase de reavaliação e expansão. O protótipo inicial utilizou potenciômetros para simular as leituras dos sensores. Agora, o foco é na seleção e integração de sensores reais, na adição de novas funcionalidades como visão computacional e na otimização do modelo de IA.

## 👥 Equipe

| Nome                 | Cargo no projeto             | GitHub                                     |
| -------------------- | ------------------------- | ------------------------------------------ |
| Silvio Azevedo    | Líder de Projeto | [@Silviozv](https://github.com/Silviozv)   |
| Yasmin Cordeiro  | Secretario de Mesa   | [@yasmincsme](https://github.com/yasmincsme)   |
| Naila Suele    | Projetista   | [@nailasuely](https://github.com/nailasuely)   |

## 🎯 Metas do Projeto

### Metas atuais

  * **Hardware:** 
  * **Software:** 

### Objetivos Gerais

- **Hardware:**
    - [x] Utilizar um ESP32 como microcontrolador principal.
    - [x] Montar Projeto na Protoboard
    - [ ] Substituir os potenciômetros de simulação por sensores reais (a serem definidos).
    - [ ] Integrar um módulo de câmara para captura de imagens.

- **Software:**
    - [x] Desenvolver um sistema de comunicação robusto entre o robô (ESP32) e um servidor central (Broker).
    - [x] Criar uma interface de usuário web para monitoramento e controle remoto.
    - [x] Implementar um sistema de captura de fotos sob demanda.
    - [x] Integrar um bot do Telegram para notificações automáticas de status.
    - [ ] Atualizar HTML e JS para colocar interface web em produção
- **IA:**
    - [x] Implementar um modelo de Inteligência Artificial embarcado no ESP32.
    - [ ] Coletar um novo dataset com os sensores reais e treinar uma versão aprimorada da rede neural.

## 🛠️ Tecnologias e Ferramentas

| Categoria      | Ferramentas                                                        |
| -------------- | ------------------------------------------------------------------ |
| **Linguagens** | C/C++ (ESP32), Python (Broker), HTML, CSS, JavaScript (Dashboard) |
| **Frameworks** | Flask, Flask-CORS (Python), Chart.js (JS), ArduinoJson, UniversalTelegramBot (C++) |
| **Hardware** | ESP32, Sensores (a serem definidos), Webcam, Módulo de Câmera (a ser definido) |
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
    * As reuniões semanais foram fixadas para o horário das 13h às 13h30 e agendadas no Google Agenda da equipe.
 
### Semana de 29/09/2025 - 05/10/2025

**Foco da Semana:** Avaliação de Artigos Envolvendo os Eixos Temáticos

* **Decisões de Projeto (Reunião de 29/09):**
    * **Avaliação dos Artigos:** O grupo discutiu alguns temas e desistiu de projetos envolvendo agricultura de precisão e green houses, decidiu-se que seria mais proveitoso explorar temas pouco publicados, como infragram.
    * **Sugestões:** Foi determinado que os integrantes do grupo irão pesquisar sobre monitoramento hídrico e técnicas de coleta de imagem que envolvam infragram.

* **Tarefas da Semana:**
    * **Yasmin:** Atualização do diário de desenvolvimento no github.
    * **Toda a equipe:** Pesquisa sobre monitoramento hídrico e infragram

### Semana de 06/10/2025 - 12/10/2025

**Foco da Semana:** Avaliação de Artigos Envolvendo Infragram e Monitoramento Hídrico

* **Decisões de Projeto (Reunião de 06/09):**
    * **Avaliação dos Artigos:** A equipe decidiu que um projeto na área de monitoramento hídrico não seria tão interessante quanto a do Infragram, que ainda é uma técnica pouco explorada.
    * **Definição do Escopo do Projeto:** Decidimos desenvolver um sistema que auxilie no monitoramento da saúde de plantas em hidropônia.

* **Tarefas da Semana:**
    * **Yasmin:** Atualização do diário de desenvolvimento no github.
    * **Toda a equipe:** Levantamento de materiais.


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
