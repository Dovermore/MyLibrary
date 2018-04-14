
# coding: utf-8

# In[117]:

class EarlyStopping():
    
    '''
    Constructor of class EarlyStopping
    Argument:
        mdrop: The number of drop before terminate the training process
        thresh: Threshold value for comparison
        operation: callable or string indicating the comparison process
                   E.g. "<" will compare current val < threshold
                   The callable need to have two input argument of (a, b)
                   and return True or False
    '''
    operations = ["<", ">", "<=", ">=", "=="]
    
    def __init__(self, mdrop, thresh=0, operation=">"):
        self.mdrop = mdrop
        self.cdrop = 0
        self.thresh = thresh
        if not (operation in self.operations or callable(operation)):
            raise TypeError("Invalid operation input. The valid operations are {}, or callable. But {} are passed.".format(self.operations, 
                           operation))
            
        self.operation = operation
        self.records = []

    
    def __compare_val(self):
        ans = False
        if not callable(self.operation):
            ans = eval("{}{}{}".format(self.records[-1], 
                                        self.operation, self.thresh))
        else:
            ans = self.operation(self.records[-1], self.thresh)
        return ans
        
    def check_val(self, new_val):
        
        self.records.append(new_val)
        if (self.__compare_val() > 0):
            self.thresh = self.records[-1]
            self.cdrop = 0
        else:
            self.cdrop += 1
        
        if self.cdrop >= self.mdrop:
            print("***\nEarly Stopping!")
            print("\tcdrop={}, mdrop={}".format(self.cdrop, self.mdrop))
            print("\tThe threshold is {} at iteration {}".format(self.thresh, 
                                            len(self.records)-self.mdrop))
            print("\tThe previous values are:", self.records[-self.mdrop:])
            return False
        return True

