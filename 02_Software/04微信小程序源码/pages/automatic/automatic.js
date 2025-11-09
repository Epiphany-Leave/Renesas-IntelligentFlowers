// pages/automatic/automatic.js
Page({
  data: {
    mainSwitch: false,
    pumpSwitch: false,
    fanSwitch: false,
    lightSwitch: false,
    selectedPlant: 0,
    plantOptions: [
      { text: '菊花', value: 0 },
      { text: '水仙花', value: 1 },
      { text: '桃花', value: 2 }
    ],
    plantInfo: [
      {
        image: '../images/菊花.png',
        description: '习性简介：\n生长环境：菊花为短日照植物，喜阳光，忌荫蔽，较耐旱，怕涝。喜温暖湿润气候，但亦能耐寒，严冬季节根茎能在地下越冬。\n温度要求：最适生长温度为20℃左右，花期最低夜温17℃，开花期（中、后）可降至15-13℃。\n土壤要求：喜地势高燥、土层深厚、富含腐殖质、轻松肥沃而排水良好的沙壤土。在微酸性到中性的土中均能生长，而以PH6.2-6.7较好。\n\n养殖技巧：\n光照：确保充足阳光，但也稍耐阴。\n水分：保持土壤湿润，但避免积水，以防根部腐烂。\n施肥：定期施肥，以促进植株生长和开花。\n修剪：适时修剪枝条，保持植株形态美观。'
      },
      {
        image: '../images/水仙花.png',
        description: '习性简介：\n生长环境：水仙花属于多年生草本植物，喜欢温暖湿润的环境，耐寒性较强。\n光照需求：需要充足的光照，但夏季高温时要避免阳光直射。\n温度管理：最适温度为15-25℃，冬季温度过低时需移至室内保暖。\n\n养殖技巧：\n光照：春秋季节放置在阳光充足处，夏季避免直射光，冬季增加光照时间。\n水分：保持盆土湿润，但不可过湿，以防根部腐烂。冬季减少浇水次数。\n施肥：生长旺盛期每隔10-15天施加一次稀薄的液肥，冬季减少施肥次数。\n病虫害防治：定期检查植株，发现病虫害及时采取措施防治。'
      },
      {
        image: '../images/桃花.png',
        description: '习性简介：\n生长环境：桃花性喜光，要求通风良好；喜排水良好，耐旱；畏涝，耐寒性较强。\n土壤要求：宜轻壤土，水分以保持半墒为好。不耐碱土，亦不喜土质过于粘重。\n\n养殖技巧：\n光照：确保充足的光照，以促进植株生长和开花。\n水分：保持土壤湿润，但避免积水，以防根部腐烂。\n施肥：定期施肥，以氮肥为主，秋季时最好追加些骨粉。\n修剪：适时修剪枝条，保持植株通风透光，促进花芽分化。\n病虫害防治：定期检查植株，发现病虫害及时采取措施防治，如使用生物防治或化学防治等方法。'
      }
    ]
  },
  onMainSwitchChange: function(event) {
    const mainSwitch = event.detail;
    if (!mainSwitch) {
      this.setData({
        pumpSwitch: false,
        fanSwitch: false,
        lightSwitch: false
      });
    } else {
      this.setPlantSwitches(this.data.selectedPlant);
    }
    this.setData({
      mainSwitch
    });
  },
  onPumpSwitchChange: function(event) {
    this.setData({
      pumpSwitch: event.detail
    });
  },
  onFanSwitchChange: function(event) {
    this.setData({
      fanSwitch: event.detail
    });
  },
  onLightSwitchChange: function(event) {
    this.setData({
      lightSwitch: event.detail
    });
  },
  prevPlant: function() {
    let newPlant = this.data.selectedPlant - 1;
    if (newPlant < 0) {
      newPlant = this.data.plantOptions.length - 1;
    }
    this.setData({
      selectedPlant: newPlant
    });
    if (this.data.mainSwitch) {
      this.setPlantSwitches(newPlant);
    }
  },
  nextPlant: function() {
    let newPlant = this.data.selectedPlant + 1;
    if (newPlant >= this.data.plantOptions.length) {
      newPlant = 0;
    }
    this.setData({
      selectedPlant: newPlant
    });
    if (this.data.mainSwitch) {
      this.setPlantSwitches(newPlant);
    }
  },
  setPlantSwitches: function(plantIndex) {
    switch (plantIndex) {
      case 0: // 菊花
        this.setData({
          pumpSwitch: true,
          fanSwitch: false,
          lightSwitch: true
        });
        break;
      case 1: // 水仙花
        this.setData({
          pumpSwitch: true,
          fanSwitch: true,
          lightSwitch: true
        });
        break;
      case 2: // 桃花
        this.setData({
          pumpSwitch: false,
          fanSwitch: true,
          lightSwitch: true
        });
        break;
    }
  }
});




