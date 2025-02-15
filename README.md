# 📄 **Protocolo Stop-and-Wait com UDP**  

## 📌 **Descrição**  
Este projeto implementa um protocolo de parada e espera (**Stop-and-Wait**) para **transmissão confiável de arquivos** utilizando **sockets UDP**. O protocolo garante que os pacotes sejam entregues corretamente, lidando com **perda de pacotes**, **retransmissões** e **eliminação de duplicatas**.  

## 📂 **Estrutura do Projeto**  
```
/
|-- cliente.c    # Implementação do cliente UDP
|-- servidor.c   # Implementação do servidor UDP
|-- README.md    # Documentação do projeto
|-- gera.py      # Script para gerar arquivos binários de tamanhos variados
```

## 🛠 **Funcionalidades**  
✔️ Envio de arquivos via UDP com confiabilidade.  
✔️ Implementação do protocolo **Stop-and-Wait** (ACK e timeout).  
✔️ Retransmissão automática em caso de perda de pacotes.  
✔️ Simulação de perda de pacotes e ACKs (probabilidade de 10%).  
✔️ Controle de duplicatas no servidor.  

## 🚀 **Como Compilar e Executar**  

### 🔧 **Compilação**  
Para compilar o cliente e o servidor, utilize o **GCC**:  
```bash
gcc cliente.c -o cliente
gcc servidor.c -o servidor
```

### 🖥 **Executando o Servidor**  
O servidor deve ser iniciado primeiro. Ele recebe os arquivos enviados pelo cliente.  
```bash
./servidor <porta>
```
Exemplo:  
```bash
./servidor 8080
```

### 📤 **Executando o Cliente**  
O cliente lê um arquivo local e envia para o servidor.  
```bash
./cliente <IP> <porta> <arquivo>
```
Exemplo:  
```bash
./cliente 127.0.0.1 8080 exemplo.txt
```

## 🔬 **Testes Realizados**  
✅ **Envio de arquivos pequenos e grandes**  
✅ **Simulação de perda de pacotes e retransmissão automática**  
✅ **Descartar pacotes duplicados corretamente**  
✅ **Arquivo reconstruído corretamente no servidor**  

## 📖 **Explicação Técnica**  
O protocolo segue a lógica **Stop-and-Wait**, onde:  
1️⃣ O cliente envia um pacote e aguarda um **ACK** do servidor.  
2️⃣ Se o ACK não for recebido dentro de um **timeout** de 2 segundos, o cliente retransmite.  
3️⃣ O servidor descarta pacotes duplicados usando o **número de sequência**.  
4️⃣ Para simular perdas, pacotes e ACKs têm **10% de chance de serem "perdidos"**.  

## 📜 **Requisitos Técnicos**  
- 🔹 **Linguagem**: C  
- 🔹 **Bibliotecas**: `sys/socket.h`, `netinet/in.h`, `arpa/inet.h`, `unistd.h`  
- 🔹 **Compilador**: GCC  
- 🔹 **Sistema Operacional**: Linux (preferencialmente)  

## 📝 **Conclusão**  
Este projeto demonstra uma solução confiável para transferência de arquivos usando **UDP**, garantindo que todos os pacotes sejam recebidos corretamente, mesmo com falhas simuladas na rede. O protocolo **Stop-and-Wait** oferece uma abordagem simples, mas eficaz para **garantir confiabilidade na transmissão de dados**. 🚀
