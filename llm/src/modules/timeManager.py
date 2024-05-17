import config
from datetime import datetime

class TimeManager(object):
    """
    Class to manage time
    """
    def getCurrentDate() -> str:
        """
        Static method to get current date
        """
        dateFormat : str = config.timeConfig["dateFormat"]

        return datetime.now().strftime(dateFormat)

    def dateEqual(date1 : str, date2 : str) -> bool:
        """
        Static method to verify if two dates are the same
        """
        dateFormat : str = config.timeConfig["dateFormat"]

        date1 : datetime = datetime.strptime(date1, dateFormat)
        date2 : datetime = datetime.strptime(date2, dateFormat)

        sameDay : bool = date1.day == date2.day
        sameMonth : bool = date1.month == date2.month
        sameYear : bool = date1.year == date2.year

        return sameDay and sameMonth and sameYear