# -*- coding: utf8 -*-
import webbrowser
import json
import os
from flask import Flask
from flask import make_response
from flask import request
from urllib2 import Request
from urllib2 import urlopen
from gevent.wsgi import WSGIServer
from geventwebsocket.handler import WebSocketHandler
from gevent import monkey
monkey.patch_all()


app=Flask(__name__)


@app.route('/file/read',methods=['GET'])
def readfile():
    print "------ Start Read File ------"
    f = open("read","r")
    try:
        i = f.read()
        print "---- Test File Says: ----"
        print i
        print "---- Read OK ----"
    finally:
        f.close()
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/write',methods=['GET'])
def writefile():
    print "------ Start Write File ------"
    f = open("write","write")
    try:
        i = f.writelines("Write OK")
        print "---- Write OK ----"
    finally:
        f.close()
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/create',methods=['GET'])
def createfile():
    print "------ Start Create File ------"
    try:
        os.mkdir('test')
    except:
    	print "---- Create New File Failed ----"
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/delete',methods=['GET'])
def deletefile():
    print "------ Start Delete File ------"
    filename = 'delete'
    if os.path.exists(filename) :
    	os.remove(filename)
    if os.path.exists(filename) :
        resp = make_response(json.dumps({'rtn':-1,'message':'Faild'}))
        resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
        return resp
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/run',methods=['GET'])
def runfile():
    print "------ Start Run File ------"
    os.system('./run')
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/network',methods=['GET'])
def network():
    print "------ Start Connect Network ------"
    webbrowser.open_new("http://www.baidu.com")
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/reboot',methods=['GET'])
def reboot():
    print "------ Start Reboot System ------"
    os.system('reboot')
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/halt',methods=['GET'])
def halt():
    print "------ Start Halt System ------"
    os.system('halt')
    resp = make_response(json.dumps({'rtn':0,'message':'OK'}))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)
