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

const storeRecord = async (id, status) => {
    const d = new Date();
    const months = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
    const citiesRef = db.collection('Rfid-Records');
    const snapshot = await citiesRef.doc().set({
        time: `${d.getHours()}:${d.getMinutes()}:${d.getSeconds()}`,
        date: `${d.getDate()} ${months[d.getMonth()]} ${d.getFullYear()}`,
        card_id: id,
        status: status
    });
    return;
}


app.post('/verify', async (req, res) => {
    const id = req.body.uid;
    console.log(id);
    const citiesRef = db.collection('rfid');
    const snapshot = await citiesRef.where('id', '==', id).get();
    if (snapshot.empty) {
        storeRecord(id, 'Access Denied');
        res.send({ status: 'Access Denied' })
        return;
    } else {
        storeRecord(id, 'Access Granted');
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
