# Proxy Arduino → Z-API

Este projeto é um proxy simples que recebe requisições HTTP (por exemplo, vindas de um Arduino) e converte para chamadas HTTPS à Z-API, uma API de envio de mensagens do WhatsApp.

## Como funciona

1. O Arduino envia uma requisição HTTP POST para o proxy.
2. O proxy recebe o payload em JSON.
3. O proxy reencaminha os dados para a Z-API usando HTTPS.
4. A resposta da Z-API é retornada ao cliente.

> O Arduino não consegue enviar diretamente para a Z-API porque o dispositivo só faz requisições HTTP simples e a Z-API exige HTTPS. Por isso é necessário usar este proxy.

## Dependências

- Node.js
- express
- axios
- dotenv

## Instalação

1. Instale as dependências:

```bash
npm install
```

2. Crie um arquivo `.env` na raiz com as variáveis abaixo:

```env
INSTANCE_ID=seu_instance_id
TOKEN=seu_token
CLIENT_TOKEN=seu_client_token
```

3. Inicie o servidor:

```bash
npm start
```

O proxy passará a rodar em `http://localhost:3000`.

## Endpoints

### `POST /send`

Envia uma mensagem via Z-API.

Corpo da requisição (JSON):

```json
{
  "phone": "5511999999999",
  "message": "Olá do Arduino!",
  "delayMessage": 15
}
```

- `phone`: número de telefone destino no formato internacional.
- `message`: texto da mensagem.
- `delayMessage`: tempo de atraso em segundos (opcional, padrão `15`).

## Exemplo de uso

O Arduino pode enviar a requisição para o proxy em HTTP e o proxy faz a conversão para HTTPS automaticamente.

## Observações

- Certifique-se de definir corretamente `INSTANCE_ID`, `TOKEN` e `CLIENT_TOKEN` no `.env`.
- O servidor usa `express.json()` para tratar o corpo em JSON.
- Erros na chamada à Z-API retornam status `500` com uma mensagem de erro.
