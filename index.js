const { async } = require('@firebase/util');
const express = require('express');
const db = require('./config');
const app = express();

const port = process.env.PORT || 3000;

app.use(express.json())
app.use(express.urlencoded({ extended: true }))

app.get('/', async (req, res) => {
    // const uid = req.body.uid;
    const citiesRef = db.collection('rfid');
    const snapshot = await citiesRef.get();
    let mydata = [];
    snapshot.forEach(doc => {
        mydata.push(doc.data());
    });
    res.send({
        result: mydata
    })
});

app.post('/verify', async (req, res) => {
    const id = req.body.uid;
    console.log(id);
    const citiesRef = db.collection('rfid');
    const snapshot = await citiesRef.where('id', '==', id).get();
    if (snapshot.empty) {
        res.send({ status: 'Access Denied' })
        return;
    } else {
        res.send({ status: 'Access Granted' })
    }
})

app.post('/', async (req, res) => {
    const uid = req.body.uid;
    // res.send(uid);
    const citiesRef = db.collection('rfid');
    const resp = await citiesRef.doc().set({
        id: `${uid}`
    });
    if (resp) {
        res.send('Data added');
    } else {
        res.send('error');
    }
});



app.listen(port, () => { console.log(`server is running at http://localhost:${port}`) });
