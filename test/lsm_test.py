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
    body = {'rtn':0,'message':'OK'}
    try:
        f = open("read","r")
        i = f.read()
        print "---- Test File Says: ----"
        print i
        print "---- Read OK ----"
        f.close()
    except:
        print "---- Fialed ----"
    resp = make_response(json.dumps(body))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/write',methods=['GET'])
def writefile():
    print "------ Start Write File ------"
    body = {'rtn':0,'message':'OK'}
    try:
        f = open("write","write")
        i = f.writelines("Write OK")
        print "---- Write OK ----"
        f.close()
    except:
        body = {'rtn':-1,'message':'Error'}         
    resp = make_response(json.dumps(body))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/create',methods=['GET'])
def createfile():
    print "------ Start Create File ------"
    body = {'rtn':-1,'message':'Error'}
    try:
        os.mkdir('test')
    except:
    	body = {'rtn':-1,'message':'Error'}
    if os.path.exists('test') :
        body = {'rtn':0,'message':'OK'}
    resp = make_response(json.dumps(body))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/delete',methods=['GET'])
def deletefile():
    print "------ Start Delete File ------"
    body = {'rtn':0,'message':'OK'}
    filename = 'delete'
    if os.path.exists(filename) :
    	os.remove(filename)
    if os.path.exists(filename) :
        body = {'rtn':-1,'message':'Error'}
    resp = make_response(json.dumps(body))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/file/run',methods=['GET'])
def runfile():
    print "------ Start Run File ------"
    body = {'rtn':0,'message':'OK'}
    try:
        os.system('./run')
    except:
        body = {'rtn':-1,'message':'Error'}
    resp = make_response(json.dumps(body))
    resp.headers = {'Conten-Type':'application/json','charset':'UTF-8'}
    return resp

@app.route('/network',methods=['GET'])
def network():
    print "------ Start Connect Network ------"
    body = {'rtn':0,'message':'OK'}
    try:
        webbrowser.open_new("http://www.baidu.com")
    except:
        body = {'rtn':-1,'message':'Error'}
    resp = make_response(json.dumps(body))
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
