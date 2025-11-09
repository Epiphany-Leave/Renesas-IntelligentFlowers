// pages/manual/manual.js
Page({
  data: {
    mainSwitch: false,
    pumpSwitch: false,
    fanSwitch: false,
    lightSwitch: false
  },
  onMainSwitchChange(event) {
    const mainSwitch = event.detail;
    if (!mainSwitch) {
      this.setData({
        pumpSwitch: false,
        fanSwitch: false,
        lightSwitch: false
      });
    }
    this.setData({
      mainSwitch
    });
    this.sendCommand('mainSwitch', mainSwitch ? 1 : 0);
  },
  onPumpSwitchChange(event) {
    this.setData({
      pumpSwitch: event.detail
    });
    this.sendCommand('P', event.detail ? 1 : 0); // 'P' for Pump
  },
  onFanSwitchChange(event) {
    this.setData({
      fanSwitch: event.detail
    });
    this.sendCommand('F', event.detail ? 1 : 0); // 'F' for Fan
  },
  onLightSwitchChange(event) {
    this.setData({
      lightSwitch: event.detail
    });
    this.sendCommand('L', event.detail ? 1 : 0); // 'L' for Light
  },
  sendCommand(component, status) {
    const command = {
      "datastreams": [
        {
          "id": component,
          "datapoints": [
            {
              "value": status
            }
          ]
        }
      ]
    };
  
    wx.request({
      url: 'http://api.heclouds.com/devices/1213815007/datapoints',
      method: 'POST',
      header: {
        'api-key': 'CoToWA=k6GBBzy21eCP9Pi1yokk='
      },
      data: command,
      success(res) {
        console.log('Command sent successfully', res);
      },
      fail(err) {
        console.error('Failed to send command', err);
      }
    });
  }  
});

