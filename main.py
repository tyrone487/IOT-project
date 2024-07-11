from flask import Flask, request, jsonify, render_template

app = Flask(__name__)

sensor_data = {
    'humedad': 0,
    'lluvia': 0
}

@app.route('/api/data', methods=['POST', 'GET'])
def receive_data():
    global sensor_data

    if request.method == 'POST':
        # Actualizar datos de los sensores
        data = request.json
        if 'humedad' in data:
            sensor_data['humedad'] = data['humedad']
        if 'lluvia' in data:
            sensor_data['lluvia'] = data['lluvia']
        return jsonify(sensor_data), 200

    elif request.method == 'GET':
        # Devolver los datos actuales del sensor en formato JSON
        return jsonify(sensor_data), 200

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)
