from flask import Flask
import time, threading, sys, os

app = Flask(__name__)

def quitAfter10Secs():
    time.sleep(10)
    os._exit(0)

if __name__ == "__main__":
    if len(sys.argv) > 1:
        if sys.argv[1] == "-q":
            threading.Thread(target=quitAfter10Secs).start()

@app.route("/")
def root():
    return "This is a test."
app.run("0.0.0.0", 8800)