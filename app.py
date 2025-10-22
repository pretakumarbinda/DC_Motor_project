from flask import Flask, render_template, request, jsonify
import requests
from threading import Thread

app = Flask(__name__)

ESP32_IP = "http://10.28.66.30"

def send_to_esp32(url):
    """Send request in background thread"""
    try:
        requests.get(url, timeout=2)
    except:
        pass  # Ignore errors, command was likely sent

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/control', methods=['POST'])
def control():
    direction = request.form.get('direction')
    speed = request.form.get('speed')
    url = f"{ESP32_IP}/control?dir={direction}&speed={speed}"
    
    # Send request in background thread (non-blocking)
    thread = Thread(target=send_to_esp32, args=(url,))
    thread.daemon = True
    thread.start()
    
    # Return immediately without waiting
    return jsonify({
        "status": "success", 
        "message": f"Command sent: {direction} @ {speed}"
    })

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True, threaded=True)