require("dotenv").config();
const express = require("express");
const axios = require("axios");

const { INSTANCE_ID, TOKEN, CLIENT_TOKEN } = process.env;

if (!INSTANCE_ID || !TOKEN || !CLIENT_TOKEN) {
    console.error("Variáveis de ambiente não foram definidas. Verifique o arquivo .env.");
    process.exit(1);
}

const app = express();
app.use(express.json());

app.post("/send", async (req, res) => {
    try {
        const { phone, message, delayMessage } = req.body;

        const url = `https://api.z-api.io/instances/${INSTANCE_ID}/token/${TOKEN}/send-text`;

        const response = await axios.post(url, {
            phone,
            message,
            delayMessage: delayMessage || 15
        }, {
            headers: {
                "Client-Token": CLIENT_TOKEN,
                "Content-Type": "application/json"
            }
        });

        res.json({
            success: true,
            data: response.data
        });

    } catch (err) {
        res.status(500).json({
            success: false,
            error: err.message
        });
    }
});

const PORT = process.env.PORT || 3000;

app.get("/", (req, res) => {
    res.send("API online");
});

app.listen(PORT, () => {
    console.log(`Proxy rodando na porta ${PORT}`);
});