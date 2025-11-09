// index.js
Page({
  data: {
    // 数据
    temp: 0,
    humi: 0,
    gas: 0,
    light: 0,
    humi_soil: 0,
    ph: 10.61,
    dataList: [
      { name: '温度', value: '0℃', icon: '../images/温度.png' },
      { name: '湿度', value: '0%', icon: '../images/湿度.png' },
      { name: '空气质量', value: '0PPM', icon: '../images/空气质量.png' },
      { name: '光照强度', value: '0LUX', icon: '../images/光照强度.png' },
      { name: '土壤湿度', value: '0%', icon: '../images/土壤湿度.png' },
      { name: 'PH值', value: '0mol/L', icon: '../images/PH.png' }
    ],
    history:{
      temp: [],
      humi: [],
      gas: [],
      light: [],
      humi_soil: [],
      ph: []      
    },

    // 图表
    chartData: {
      categories:["初始化中..."],
      series:[{data:[0], color:"#CCCCCC"}],
    },
    chartOpts: {
      color: ["#1890FF", "#91CB74", "#FAC858", "#EE6666", "#73C0DE", "#3BA272"],
      animation: false,  // 关闭动画
      dataLabel: false,
      enableScroll: true,
      xAxis: { 
        scrollAlign: 'left',
        // itemCount: Math.min(history.temp.length, 10), //
        itemCount: 10,
        disableGrid: true
      },
      yAxis: { gridType: "dash" },
      extra: { line: { type: "curve", width: 2 , animation:false} }
    }    
  },

  // 事件处理函数
  getinfo() {
    var that = this;
    wx.request({
      url: "https://api.heclouds.com/devices/1213815007/datastreams",
      _header: {
        "api-key": "CoToWA=k6GBBzy21eCP9Pi1yokk="
      },
      get header() {
        return this._header;
      },
      set header(value) {
        this._header = value;
      },
      method: "GET",
      success: function(e) {
        console.log("获取成功", e);
        const temp = e.data.data[7].current_value;
        const humi = e.data.data[8].current_value;
        const gas = e.data.data[4].current_value;
        const light = e.data.data[3].current_value;
        const humi_soil = e.data.data[5].current_value;
        const ph = e.data.data[6].current_value;

        that.setData({
          temp,
          humi,
          gas,
          light,
          humi_soil,
          dataList: [
            { name: '温度', value: `${temp}℃`, icon: '../images/温度.png' },
            { name: '湿度', value: `${humi}%`, icon: '../images/湿度.png' },
            { name: '空气质量', value: `${gas}PPM`, icon: '../images/空气质量.png' },
            { name: '光照强度', value: `${light}LUX`, icon: '../images/光照强度.png' },
            { name: '土壤湿度', value: `${humi_soil}%`, icon: '../images/土壤湿度.png' },
            { name: 'PH值', value: `${ph}mol/L`, icon: '../images/PH.png' }
          ]
        });

        console.log("temp==", that.data.temp);
        console.log("humi==", that.data.humi);
        console.log("gas==", that.data.gas);
        console.log("light==", that.data.light);
        console.log("humi_soil==", that.data.humi_soil);
        console.log("ph==", that.data.ph);

        const now = new Date().getTime(); // 获取当前时间戳
  
        // 更新历史数据（保持1024长度）
        that.updateHistoryData('temp', temp, now);
        that.updateHistoryData('humi', humi, now);
        that.updateHistoryData('gas', gas, now);
        that.updateHistoryData('light', light, now);
        that.updateHistoryData('humi_soil', humi_soil, now);
        that.updateHistoryData('ph', ph, now);
        // 其他参数同理...
      
        // 可选：定期同步到本地缓存（每5分钟）
        if(Date.now() % 300000 < 1000) {
          wx.setStorageSync('sensor_history', that.data.history);
        }
      }
    });
  },

  // 维护历史数据队列
  updateHistoryData(key, value, timestamp) {
    const history = this.data.history[key];
    if(history.length >= 1024) {
      history.shift(); // 移除最早数据
    }
    history.push({ value, timestamp }); // 添加新数据
    this.setData({
      [`history.${key}`]: history
    });
  },

  // 初始化历史数据（在onLoad中调用）
  initHistoryData() {
    const cached = wx.getStorageSync('sensor_history');
    console.log('读取缓存:',cached); // debug
    if(cached) {
      this.setData({ history: cached });
    }
  },  

  updateChart() {

      console.log("当前图表数据：", this.data.chartData);
      const history = this.data.history;
      const hasData = history.temp.length > 0;
      const chartData = hasData ? {
        categories: history.temp.slice(-10).map(item => 
          new Date(item.timestamp).toLocaleTimeString().substr(0,5)),
        series: [
          { name: "温度", data: history.temp.map(i => Number(i.value)) },
          { name: "湿度", data: history.humi.map(i => Number(i.value)) },
          { name: "空气质量", data: history.gas.map(i => Number(i.value)) },
          { name: "光照强度", data: history.light.map(i => Number(i.value)) },
          { name: "土壤湿度", data: history.humi_soil.map(i => Number(i.value)) },
          { name: "PH值", data: history.ph.map(i => Number(i.value)) },
          // 其他数据系列...
        ]
      } : {
        categories:["等待数据"],
        series:[{data:[0]}],
      };
      this.setData({ 
        chartData: JSON.parse(JSON.stringify(chartData)),
        update: true 
      });
  },

  onLoad() {
    this.initHistoryData();
    var that = this;
    setTimeout(() =>{
      let updataFlag = 0;
      setInterval(function() {
        that.getinfo();
        if(updataFlag++ % 3 === 0){
          that.updateChart();
        }
      }, 1000);
    }, 100)
  },
});

  
  