import re
import subprocess

from flask import Flask
from flask import jsonify
from flask import request

app = Flask(__name__)


@app.route('/')
def show_user_ip():
    user_ip = request.remote_addr
    return f'{user_ip}'


@app.route('/ping', methods=['PING'])
def ping_ip():
    ip_address = request.form.get('ip')

    blacklist = [
        ';', '&', '>', '<', 'flag', 'rm', 'tee', 'shutdown', 'reboot', 'exit'
    ]
    if any(word in ip_address for word in blacklist):
        return jsonify({"error": "Invalid characters in IP address"}), 400

    if not ip_address:
        return jsonify({"error": "No IP address provided"}), 400

    ip_regex = r"\b(?:[0-9]{1,3}\.){3}[0-9]{1,3}\b"
    if not re.match(ip_regex, ip_address):
        return jsonify({"error": "Invalid IP address format"}), 400

    result = subprocess.run(
        f"ping -c 4 {ip_address}",
        stdout=subprocess.PIPE,
        text=True,
        shell=True
    )

    return jsonify({"result": result.stdout})


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=1337)

