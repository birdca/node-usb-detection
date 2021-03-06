#include "detection.h"


#define OBJECT_ITEM_LOCATION_ID     "locationId"
#define OBJECT_ITEM_VENDOR_ID       "vendorId"
#define OBJECT_ITEM_PRODUCT_ID      "productId"
#define OBJECT_ITEM_DEVICE_NAME     "deviceName"
#define OBJECT_ITEM_MANUFACTURER    "manufacturer"
#define OBJECT_ITEM_SERIAL_NUMBER   "serialNumber"
#define OBJECT_ITEM_DEVICE_ADDRESS  "deviceAddress"


NanCallback* addedCallback;
bool isAddedRegistered = false;

NanCallback* removedCallback;
bool isRemovedRegistered = false;

void RegisterAdded(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  v8::Local<v8::Function> callback;

  if (args.Length() == 0) 
  {
    return NanThrowTypeError("First argument must be a function");
  }

  if (args.Length() == 1) 
  {
    // callback
    if(!args[0]->IsFunction()) 
    {
      return NanThrowTypeError("First argument must be a function");
    }

    callback = args[0].As<v8::Function>();
  }

  addedCallback = new NanCallback(callback);
  isAddedRegistered = true;
}

void NotifyAdded(ListResultItem_t* it) 
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  if (isAddedRegistered)
  {
    v8::Handle<v8::Value> argv[1];
    v8::Local<v8::Object> item = v8::Object::New(isolate);
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_LOCATION_ID), v8::Number::New(isolate, it->locationId));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_VENDOR_ID), v8::Number::New(isolate, it->vendorId));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_PRODUCT_ID), v8::Number::New(isolate, it->productId));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_DEVICE_NAME), v8::String::NewFromUtf8(isolate, it->deviceName.c_str()));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_MANUFACTURER), v8::String::NewFromUtf8(isolate, it->manufacturer.c_str()));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_SERIAL_NUMBER), v8::String::NewFromUtf8(isolate, it->serialNumber.c_str()));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_DEVICE_ADDRESS), v8::Number::New(isolate, it->deviceAddress));
    argv[0] = item;
    //v8::Function::Cast(*addedCallback)->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    //v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, addedCallback);
    //NanCallback *callback = new NanCallback(cb);
    //v8::Handle<v8::Value> nan_argv[] = {
    //  NanNew<v8::Object>(argv[0])
    //};
    addedCallback->Call(1, argv);
  }
}

void RegisterRemoved(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  v8::Local<v8::Function> callback;

  if (args.Length() == 0) 
  {
    return NanThrowTypeError("First argument must be a function");
  }

  if (args.Length() == 1) 
  {
    // callback
    if(!args[0]->IsFunction()) 
    {
      return NanThrowTypeError("First argument must be a function");
    }

    callback = args[0].As<v8::Function>();
  }

  removedCallback = new NanCallback(callback);
  isRemovedRegistered = true;
}

void NotifyRemoved(ListResultItem_t* it) 
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  if (isRemovedRegistered) 
  {
    v8::Handle<v8::Value> argv[1];
    v8::Local<v8::Object> item = v8::Object::New(isolate);
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_LOCATION_ID), v8::Number::New(isolate, it->locationId));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_VENDOR_ID), v8::Number::New(isolate, it->vendorId));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_PRODUCT_ID), v8::Number::New(isolate, it->productId));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_DEVICE_NAME), v8::String::NewFromUtf8(isolate, it->deviceName.c_str()));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_MANUFACTURER), v8::String::NewFromUtf8(isolate, it->manufacturer.c_str()));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_SERIAL_NUMBER), v8::String::NewFromUtf8(isolate, it->serialNumber.c_str()));
    item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_DEVICE_ADDRESS), v8::Number::New(isolate, it->deviceAddress));
    argv[0] = item;
    //v8::Function::Cast(*removedCallback)->Call(v8::Context::GetCurrent()->Global(), 1, argv);
    //v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, removedCallback);
    //NanCallback *callback = new NanCallback(cb);
    removedCallback->Call(1, argv);
  }
}

void Find(const v8::FunctionCallbackInfo<v8::Value>& args) 
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  int vid = 0;
  int pid = 0;
  v8::Local<v8::Function> callback;

  if (args.Length() == 0) 
  {
    return NanThrowTypeError("First argument must be a function");
  }

  if (args.Length() == 3) 
  {
    if (args[0]->IsNumber() && args[1]->IsNumber()) 
    {
        vid = (int) args[0]->NumberValue();
        pid = (int) args[1]->NumberValue();
    }

    // callback
    if(!args[2]->IsFunction()) 
    {
      return NanThrowTypeError("Third argument must be a function");
    }

    callback = args[2].As<v8::Function>();
  }

  if (args.Length() == 2) 
  {
    if (args[0]->IsNumber()) 
    {
        vid = (int) args[0]->NumberValue();
    }

    // callback
    if(!args[1]->IsFunction()) 
    {
      return NanThrowTypeError("Second argument must be a function");
    }

    callback = args[1].As<v8::Function>();
  }

  if (args.Length() == 1) 
  {
    // callback
    if(!args[0]->IsFunction()) 
    {
      return NanThrowTypeError("First argument must be a function");
    }

    callback = args[0].As<v8::Function>();
  }

  ListBaton* baton = new ListBaton();
  strcpy(baton->errorString, "");
  baton->callback = new NanCallback(callback);
  //baton->callback = v8::Persistent<v8::Value>::New(isolate, callback);
  //NanAssignPersistent(baton->callback, new NanCallback(callback));
  baton->vid = vid;
  baton->pid = pid;

  uv_work_t* req = new uv_work_t();
  req->data = baton;
  uv_queue_work(uv_default_loop(), req, EIO_Find, (uv_after_work_cb)EIO_AfterFind);
}

void EIO_AfterFind(uv_work_t* req) 
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  ListBaton* data = static_cast<ListBaton*>(req->data);

  v8::Handle<v8::Value> argv[2];
  if(data->errorString[0]) 
  {
    argv[0] = v8::Exception::Error(v8::String::NewFromUtf8(isolate, data->errorString));
    argv[1] = NanUndefined();
  }
  else 
  {
    v8::Local<v8::Array> results = v8::Array::New(isolate);
    int i = 0;
    for(std::list<ListResultItem_t*>::iterator it = data->results.begin(); it != data->results.end(); it++, i++) {
      v8::Local<v8::Object> item = v8::Object::New(isolate);
      item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_LOCATION_ID), v8::Number::New(isolate, (*it)->locationId));
      item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_VENDOR_ID), v8::Number::New(isolate, (*it)->vendorId));
      item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_PRODUCT_ID), v8::Number::New(isolate, (*it)->productId));
      item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_DEVICE_NAME), v8::String::NewFromUtf8(isolate, (*it)->deviceName.c_str()));
      item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_MANUFACTURER), v8::String::NewFromUtf8(isolate, (*it)->manufacturer.c_str()));
      item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_SERIAL_NUMBER), v8::String::NewFromUtf8(isolate, (*it)->serialNumber.c_str()));
      item->Set(v8::String::NewFromUtf8(isolate, OBJECT_ITEM_DEVICE_ADDRESS), v8::Number::New(isolate, (*it)->deviceAddress));
      results->Set(i, item);
    }
    argv[0] = NanUndefined();
    argv[1] = results;
  }
  //v8::Function::Cast(*data->callback)->Call(v8::Context::GetCurrent()->Global(), 2, argv);
  //v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, data->callback.As<v8::Function>());
  //NanCallback *callback = new NanCallback(cb);
  data->callback->Call(2, argv);

  //data->callback.Dispose();
  for(std::list<ListResultItem_t*>::iterator it = data->results.begin(); it != data->results.end(); it++) 
  {
    delete *it;
  }
  delete data;
  delete req;
}

void StartMonitoring(const v8::FunctionCallbackInfo<v8::Value>& args) 
{
  Start();
}

void StopMonitoring(const v8::FunctionCallbackInfo<v8::Value>& args) 
{
  Stop();
}

extern "C" {
  void init (v8::Handle<v8::Object> target) 
  {
    NODE_SET_METHOD(target, "find", Find);
    NODE_SET_METHOD(target, "registerAdded", RegisterAdded);
    NODE_SET_METHOD(target, "registerRemoved", RegisterRemoved);
    NODE_SET_METHOD(target, "startMonitoring", StartMonitoring);
    NODE_SET_METHOD(target, "stopMonitoring", StopMonitoring);
    InitDetection();
  }
}

NODE_MODULE(detection, init);
