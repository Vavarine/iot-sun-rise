export type Alarm = {
  time: {
    hour: number;
    minute: number;
  };
  days: {
    sunday: boolean;
    monday: boolean;
    tuesday: boolean;
    wednesday: boolean;
    thursday: boolean;
    friday: boolean;
    saturday: boolean;
  };
  enabled: boolean;
};
