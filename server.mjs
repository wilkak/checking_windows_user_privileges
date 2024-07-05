import express from 'express';
import open from 'open';
import path from 'path';
import { fileURLToPath } from 'url';
import { createRequire } from 'module';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const require = createRequire(import.meta.url);
const { checkUserPrivilege } = require('./native/build/Release/userprivilege.node');

const app = express();
const PORT = process.env.PORT || 3000;

app.set('view engine', 'ejs');
app.set('views', path.join(__dirname, 'public', 'views'));

app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.render('index');
});

app.post('/check', (req, res) => {
    const username = req.body.username;
    const privilege = checkUserPrivilege(username);

    let message;
    if (privilege === "Пользователя нет") {
        message = `Пользователя ${username} нет`;
    } else {
        message = `Пользователь ${username} имеет привилегию ${privilege}`;
    }

    res.render('result', { message, username, privilege });
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
    open(`http://localhost:${PORT}`);
}).on('error', (err) => {
    if (err.code === 'EADDRINUSE') {
        console.error(`Port ${PORT} is already in use. Please choose another port.`);
    } else {
        console.error(err);
    }
});
